/*
 * SM5424 charger driver
 * NOTE: Some code was partially copied from smb1351-charger.c!
 * ---------------------
 * Copyright (c) 2021, MeizuCustoms (Roman Rihter)
 */

#include "sm5424-charger.h"
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/pm.h>
#include <linux/pm_wakeup.h>
#include <linux/power_supply.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>

static int sm5424_read_reg(struct sm5424_charger *sm5424, int reg,
                           unsigned char *val) {
  unsigned int ret = 0;

  pm_stay_awake(sm5424->dev);

  ret = i2c_smbus_read_byte_data(sm5424->client, reg);

	pm_relax(sm5424->dev);

  if (ret < 0) {
    pr_err("sm5424: i2c: failed to read register 0x%02x.\n", reg);
    return ret;
  }

	*val = ret;
	sm5424_dbg("reg: 0x%02x, val: 0x%02lx\n", reg, (unsigned long)val);
	return 0;
}

static int sm5424_write_reg(struct sm5424_charger *sm5424, int reg,
                            unsigned char val) {
  int ret = 0;

	sm5424_dbg("reg: 0x%02x, val: 0x%02x\n", reg, (unsigned int)val);

  pm_stay_awake(sm5424->dev);

  ret = i2c_smbus_write_byte_data(sm5424->client, reg, val);

	pm_relax(sm5424->dev);

  if (ret < 0) {
    pr_err("sm5424: i2c: failed to write value 0x%02x to register 0x%02x.\n",
           val, reg);
    return ret;
  }

	return 0;
}

static int sm5424_masked_write(struct sm5424_charger *sm5424, int reg,
                               unsigned char mask, unsigned char val) {
  unsigned int ret = 0;
  unsigned char temp;

  sm5424_dbg("reg: 0x%02x, mask: 0x%02x, val: 0x%02x\n", reg,
             (unsigned int)mask, (unsigned int)val);
  ret = sm5424_read_reg(sm5424, reg, &temp);
  if (ret) {
    pr_err("sm5424: masked write: failed to read reg 0x%02x.\n", reg);
    return ret;
  }

  temp &= ~mask;
  temp |= val & mask;

  ret = sm5424_write_reg(sm5424, reg, temp);
  if (ret) {
    pr_err("sm5424: masked write: failed to write reg 0x%02x.\n", reg);
    return ret;
  }

  return 0;
};

static int sm5424_usb_suspend(struct sm5424_charger *sm5424, int reason,
                              bool suspend) {
  int ret = 0;
  int suspended;
  unsigned char buf = 0x10;

  suspended = sm5424->usb_suspended_status;

  sm5424_dbg("reason = %d requested_suspend = %d suspended_status = %d\n",
             reason, suspend, suspended);

  if (suspend == false)
    suspended &= ~reason;
  else
    suspended |= reason;

  if (sm5424->is_otg_boost || !suspended) {
    buf = 0x0;
  }

  sm5424_dbg("new suspended_status = %d\n", suspended);

  ret = sm5424_masked_write(sm5424, SM5424_CNTL, 0x10, buf);
  if (ret)
    pr_err("sm5424: couldn't suspend (%d)\n", ret);
  else
    sm5424->usb_suspended_status = suspended;

  return ret;
}

static int sm5424_get_max_current(struct sm5424_charger *sm5424) {
  unsigned char val;
  int max_current = 100;
  int ret;

  if (!sm5424->parallel_charger_present)
    return 0;

  ret = sm5424_read_reg(sm5424, SM5424_VBUSCNTL, &val);
  if (ret) {
    pr_err("sm5424: get_max_current: failed to read VBUSCNTL.\n");
    return 0;
  }

  if (val)
    max_current = 25 * (val - 1) + 375;

  sm5424_dbg("max current - %d.\n", max_current * 1000);

  if ((max_current - 100) <= 3425) {
    if (max_current <= 0)
      return 0;

    return max_current * 1000;
  }

  return 0;
}

static int sm5424_get_battery_status(struct sm5424_charger *sm5424) {
  unsigned char status[3];
  int ret;

  ret = sm5424_read_reg(sm5424, SM5424_STATUS1, &status[0]);
  if (ret) {
    pr_err("sm5424: get_batt_status: failed to read STATUS1.\n");
    return POWER_SUPPLY_STATUS_UNKNOWN;
  }

  ret = sm5424_read_reg(sm5424, SM5424_STATUS2, &status[1]);
  if (ret) {
    pr_err("sm5424: get_batt_status: failed to read STATUS2.\n");
    return POWER_SUPPLY_STATUS_UNKNOWN;
  }

  ret = sm5424_read_reg(sm5424, SM5424_STATUS3, &status[2]);
  if (ret) {
    pr_err("sm5424: get_batt_status: failed to read STATUS3.\n");
    return POWER_SUPPLY_STATUS_UNKNOWN;
  }

  sm5424_dbg("st1: %d, st2: %d, st3: %d\n", status[0] & 1, status[1] & 1,
             status[2] & 1);

  if (!(status[1] & 1)) {
    if ((status[0] & 1))
      return POWER_SUPPLY_STATUS_NOT_CHARGING;
    else
      return POWER_SUPPLY_STATUS_DISCHARGING;
  }

  return POWER_SUPPLY_STATUS_CHARGING;
}

static int sm5424_is_input_current_enabled(struct sm5424_charger *sm5424) {
  int ret;
  unsigned char val;

  ret = sm5424_read_reg(sm5424, SM5424_STATUS1, &val);
  if (ret) {
    pr_err("sm5424: is_input_current_enabled: failed to read STATUS1.\n");
    return 0;
  }

  sm5424_dbg("enabled: %d", (val >> 3) & 1);

  return (val >> 3) & 1;
}

static int sm5424_charger_get_property(struct power_supply *psy,
                                       enum power_supply_property prop,
                                       union power_supply_propval *val) {
  struct sm5424_charger *sm5424 = power_supply_get_drvdata(psy);

  if (!sm5424->parallel_charger_present) {
    switch (prop) {
    case POWER_SUPPLY_PROP_PRESENT:
      val->intval = sm5424->parallel_charger_present;
      break;
    case POWER_SUPPLY_PROP_STATUS:
      val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
      break;
    case POWER_SUPPLY_PROP_VOLTAGE_MAX:
      val->intval = sm5424->vfloat_mv;
      break;
    case POWER_SUPPLY_PROP_CHARGING_ENABLED:
      val->intval = !sm5424->usb_suspended_status;
      break;
    default:
      val->intval = 0;
      break;
    }

    return 0;
  }

  switch (prop) {
  case POWER_SUPPLY_PROP_CURRENT_MAX:
    val->intval = sm5424_get_max_current(sm5424);
    break;
  case POWER_SUPPLY_PROP_PRESENT:
    val->intval = sm5424->parallel_charger_present;
    break;
  case POWER_SUPPLY_PROP_VOLTAGE_MAX:
    val->intval = sm5424->vfloat_mv;
    break;
  case POWER_SUPPLY_PROP_STATUS:
    val->intval = sm5424_get_battery_status(sm5424);
    break;
  case POWER_SUPPLY_PROP_CHARGING_ENABLED:
    val->intval = !sm5424->usb_suspended_status;
    break;
  case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMITED:
    val->intval = sm5424_is_input_current_enabled(sm5424);
    break;
  case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
    val->intval = sm5424->fastchg_current_max_ma;
    break;
  default:
    return -EINVAL;
  }

  return 0;
}

static int sm5424_set_max_current(struct sm5424_charger *sm5424,
                                  int new_current) {
  int ret;
  int val = 0;

  sm5424_dbg("new: %d\n", new_current);

  if (new_current < 2) {
    sm5424_usb_suspend(sm5424, CURRENT, 1);
    return 0;
  }

  if (new_current > 99) {
    if (new_current > 3525)
      new_current = 3525;

    if (new_current >= 375)
      val = (new_current - 375) / 25 + 1;
    else
      val = 0;
  }

  ret = sm5424_masked_write(sm5424, SM5424_VBUSCNTL,
                            SM5424_VBUSCNTL_VBUSLIMIT_MASK, val);
  if (ret) {
    pr_err("sm5424: failed to change VBUSLIMIT. (%d)\n", ret);
    return 0;
  }

  sm5424_usb_suspend(sm5424, CURRENT, 0);

  return 0;
}

static int sm5424_set_otg_boost(struct sm5424_charger *sm5424, bool enabled) {
  int ret;

  sm5424_dbg("enabled: %d\n", enabled);

  ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL5,
                            SM5424_CHGCNTL5_OTGCURRENT_MASK, 8);
  if (ret) {
    pr_err("sm5424: failed to write to CHGCNTL5.\n");
    return ret;
  }

  ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL6, SM5424_CHGCNTL6_VOTG_MASK,
                            0);
  if (ret) {
    pr_err("sm5424: failed to write to CHGCNTL6.\n");
    return ret;
  }

  if (enabled) {
    sm5424->is_otg_boost = 1;
    sm5424_usb_suspend(sm5424, OTG, 0);

    ret = sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_CHGEN_MASK, 0);
    if (ret) {
      pr_err("sm5424: failed to write to CNTL.\n");
      return ret;
    }

    sm5424->battchg_disabled_status |= 0x40;

    ret = sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_OTGEN_MASK, 4);
    if (ret) {
      pr_err("sm5424: failed to write to CNTL.\n");
      return ret;
    }
  } else {
    sm5424->is_otg_boost = 0;

    ret = sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_OTGEN_MASK, 4);
    if (ret) {
      pr_err("sm5424: failed to write to CNTL.\n");
      return ret;
    }

    sm5424->battchg_disabled_status &= -0x41;

    ret = sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_CHGEN_MASK,
                              !sm5424->battchg_disabled_status);
    if (ret) {
      pr_err("sm5424: failed to write to CNTL.\n");
      return ret;
    }

    sm5424_usb_suspend(sm5424, OTG, 1);
  }

  return 0;
}

static int sm5424_fast_charge_current_set(struct sm5424_charger *sm5424,
                                          int new_current) {
  int ret;

  sm5424_dbg("new: %d\n", new_current);

  mutex_lock(&sm5424->fcc_lock);

  if (new_current < 350) {
    new_current = 350;
  } else if (new_current > 3500) {
    new_current = 3500;
  }

  sm5424->fastchg_current_max_ma = new_current;

  mutex_unlock(&sm5424->fcc_lock);

  ret =
      sm5424_masked_write(sm5424, SM5424_CHGCNTL2, SM5424_CHGCNTL2_FASTCHG_MASK,
                          (new_current - 350) / 50);
  if (ret) {
    pr_err("sm5424: failed to write to CHGCNTL2.\n");
    return ret;
  }

  return 0;
}

static int sm5424_set_charge_present(struct sm5424_charger *sm5424,
                                     bool present) {
  int ret;
  unsigned char devid, recharge_reg, iterm_reg;

  sm5424_dbg("present: %d\n", present);

  if (present == sm5424->parallel_charger_present)
    return 0;

  if (present) {
    ret = sm5424_read_reg(sm5424, SM5424_DEVICEID, &devid);
    if (ret) {
      pr_err("sm5424: failed to read device id (%d).\n", ret);
      return ret;
    }

    sm5424->version = devid;

    pr_info("sm5424: chip revision 0x%02x\n", devid);

    // Reset interrupt masks
    ret = sm5424_write_reg(sm5424, SM5424_INTMASK1, 0xFF);
    if (ret) {
      pr_err("sm5424: failed to write to INTMASK1 (%d).\n", ret);
      return ret;
    }

    ret = sm5424_write_reg(sm5424, SM5424_INTMASK2, 0xFF);
    if (ret) {
      pr_err("sm5424: failed to write to INTMASK2 (%d).\n", ret);
      return ret;
    }

    ret = sm5424_write_reg(sm5424, SM5424_INTMASK3, 0xFF);
    if (ret) {
      pr_err("sm5424: failed to write to INTMASK3 (%d).\n", ret);
      return ret;
    }

    if (sm5424->vfloat_mv > 4620) {
      pr_err("sm5424: too big vfloat voltage: %d.\n", sm5424->vfloat_mv);
      return ret;
    }

    ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL3,
                              SM5424_CHGCNTL3_BATREG_MASK,
                              (sm5424->vfloat_mv - 3990) / 10);
    if (ret) {
      pr_err("sm5424: failed to write to CHGCNTL3 (%d).\n", ret);
      return ret;
    }

    ret =
        sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_RECHGREG_MASK, 2);
    if (ret) {
      pr_err("sm5424: failed to write to CNTL (%d).\n", ret);
      return ret;
    }

    if (sm5424->recharge_mv <= 50)
      recharge_reg = 0;
    else
      recharge_reg = 64;

    ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL2,
                              SM5424_CHGCNTL2_RECHG_MASK, recharge_reg);
    if (ret) {
      pr_err("sm5424: failed to write to CHGCNTL2 (%d).\n", ret);
      return ret;
    }

    sm5424->target_fastchg_current_max_ma = 350;
    sm5424_fast_charge_current_set(sm5424, 350);

    ret = sm5424_usb_suspend(sm5424, SOC, 0);
    if (ret)
      return ret;

    ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL4,
                              SM5424_CHGCNTL4_FREQSEL_MASK,
                              16 * sm5424->switch_freq);
    if (ret) {
      pr_err("sm5424: failed to write to CHGCNTL4 (%d).\n", ret);
      return ret;
    }

    if (sm5424->iterm_ma > 99) {
      if (sm5424->iterm_ma < 475) {
        iterm_reg = (sm5424->iterm_ma - 100) / 25;
      } else {
        iterm_reg = 15;
      }
    } else {
      iterm_reg = 0;
    }

    ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL4,
                              SM5424_CHGCNTL4_TOPOFF_MASK, iterm_reg);
    if (ret) {
      pr_err("sm5424: failed to write to CHGCNTL4 (%d).\n", ret);
      return ret;
    }

  } else {
    ret = sm5424_usb_suspend(sm5424, SOC, 1);
    if (ret)
      return ret;
  }

  return 0;
}

static int sm5424_set_max_voltage(struct sm5424_charger *sm5424, int voltage) {
  int ret;

  sm5424_dbg("voltage: %d\n", voltage);

  if (voltage == sm5424->vfloat_mv) {
    return ret;
  }

  if ((voltage - 3990) > 630) {
    pr_err("sm5424: too big vfloat voltage. (%d)\n", voltage);
    return ret;
  }

  ret = sm5424_masked_write(sm5424, SM5424_CHGCNTL3,
                            SM5424_CHGCNTL3_BATREG_MASK, (voltage - 3990) / 10);
  if (ret) {
    pr_err("sm5424: failed to write to CHGCNTL3. (%d)\n", ret);
  }

  return ret;
}

static int sm5424_charger_set_property(struct power_supply *psy,
                                       enum power_supply_property prop,
                                       const union power_supply_propval *val) {
  struct sm5424_charger *sm5424 = power_supply_get_drvdata(psy);
  int ret;

  if (!sm5424->parallel_charger_present && prop != POWER_SUPPLY_PROP_PRESENT) {
    switch (prop) {
    case POWER_SUPPLY_PROP_PRESENT:
      sm5424_set_charge_present(sm5424, val->intval);
      break;
    case POWER_SUPPLY_PROP_VOLTAGE_MAX:
      sm5424->vfloat_mv = val->intval;
      break;
    default:
      break;
    }

    return 0;
  }

  switch (prop) {
  case POWER_SUPPLY_PROP_CURRENT_MAX:
    sm5424_set_max_current(sm5424, val->intval / 1000);
    break;
  case POWER_SUPPLY_PROP_USB_OTG:
    sm5424_set_otg_boost(sm5424, val->intval);
    break;
  case POWER_SUPPLY_PROP_CHARGING_ENABLED:
    ret = sm5424_usb_suspend(sm5424, USER, !val->intval);
    if (ret)
      pr_err("sm5424: Failed to change sm5424 state (%d)\n", ret);
    break;
  case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
    sm5424_fast_charge_current_set(sm5424, val->intval);
    break;
  case POWER_SUPPLY_PROP_PRESENT:
    sm5424_set_charge_present(sm5424, val->intval);
    break;
  case POWER_SUPPLY_PROP_VOLTAGE_MAX:
    sm5424_set_max_voltage(sm5424, val->intval);
    break;
  default:
    return -EINVAL;
  }

  return 0;
}

static int sm5424_charger_parse_dts(struct i2c_client *client,
                                    struct sm5424_charger *sm5424) {
  const struct device_node *of = client->dev.of_node;
  int ret = 0;

  if (!of_property_read_bool(of, "qcom,parallel-charger")) {
    pr_err("sm5424: dts: parallel-charger property wasn't found.\n");
    return -EFAULT;
  }

  sm5424->client = client;
  sm5424->dev = &client->dev;
  sm5424->is_slave = true;
  sm5424->usb_suspended_status =
      of_property_read_bool(of, "qcom,charging-disabled");

  ret = of_property_read_u32(of, "qcom,float-voltage-mv", &sm5424->vfloat_mv);
  if (ret) {
    pr_err("sm5424: dts: failed to read float voltage.\n");
    goto err;
  }

  ret = of_property_read_u32(of, "qcom,recharge-mv", &sm5424->recharge_mv);
  if (ret) {
    pr_err("sm5424: dts: failed to read recharge voltage.\n");
    goto err;
  }

  ret = of_property_read_u32(of, "qcom,nINT-gpio", &sm5424->irq_gpio);
  if (ret) {
    pr_err("sm5424: dts: failed to read IRQ GPIO.\n");
    goto err;
  }

  // Also request IRQ
  sm5424->irq = gpio_to_irq(sm5424->irq_gpio);

  ret = of_property_read_u32(of, "qcom,switch-freq", &sm5424->switch_freq);
  if (ret) {
    pr_err("sm5424: dts: failed to read switch frequency.\n");
    goto err;
  }

  ret = of_property_read_u32(of, "qcom,iterm-ma", &sm5424->iterm_ma);
  if (ret) {
    pr_err("sm5424: dts: failed to read iterm mA.\n");
    goto err;
  }

  return 0;
err:
  return ret;
}

static int sm5424_charger_is_writeable(struct power_supply *psy,
                                       enum power_supply_property prop) {
  switch (prop) {
  case POWER_SUPPLY_PROP_CHARGING_ENABLED:
    return 1;
  default:
    return 0;
  }
}

static int sm5424_charger_probe(struct i2c_client *client,
                                const struct i2c_device_id *id) {
  int ret = 0;
  struct sm5424_charger *sm5424;
	struct power_supply_config parallel_psy_cfg = {};

  sm5424 = devm_kzalloc(&client->dev, sizeof(*sm5424), GFP_KERNEL);
  if (!sm5424) {
    pr_err("sm5424: failed to allocate memory for main struct.\n");
    return -ENOMEM;
  }

  ret = sm5424_charger_parse_dts(client, sm5424);
  if (ret) {
    pr_err("sm5424: failed to parse dts. (%d)\n", ret);
    return ret;
  }

  i2c_set_clientdata(client, sm5424);

  sm5424->parallel_psy_d.name = "usb-parallel";
  sm5424->parallel_psy_d.type = POWER_SUPPLY_TYPE_PARALLEL;
  sm5424->parallel_psy_d.get_property = sm5424_charger_get_property;
  sm5424->parallel_psy_d.set_property = sm5424_charger_set_property;
  sm5424->parallel_psy_d.properties = sm5424_psy_properties;
  sm5424->parallel_psy_d.property_is_writeable = sm5424_charger_is_writeable;
  sm5424->parallel_psy_d.num_properties = ARRAY_SIZE(sm5424_psy_properties);

	parallel_psy_cfg.drv_data = sm5424;
	parallel_psy_cfg.num_supplicants = 0;

  mutex_init(&sm5424->fcc_lock);

  sm5424->parallel_psy = devm_power_supply_register(sm5424->dev, 
                        &sm5424->parallel_psy_d, &parallel_psy_cfg);
  if (IS_ERR(sm5424->parallel_psy)) {
		pr_err("sm5424: Couldn't register parallel psy (%ld)\n",
				PTR_ERR(sm5424->parallel_psy));
		return ret;
	}

  sm5424->is_otg_boost = 0;
  ret = sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_OTGEN_MASK, 0);
  if (ret) {
    pr_err("sm5424: Failed to disable OTG (%d).\n", ret);
  }

  sm5424_masked_write(sm5424, SM5424_CNTL, SM5424_CNTL_CHGEN_MASK, 1);

  pr_info("sm5424: probed\n");

  return 0;
}

static int sm5424_charger_remove(struct i2c_client *client) {
  return 0;
}

static int sm5424_suspend(struct device *dev) { return 0; }

static int sm5424_suspend_noirq(struct device *dev) { return 0; }

static int sm5424_resume(struct device *dev) { return 0; }

static struct of_device_id sm5424_match_table[] = {
    {
        .compatible = "qcom,sm5424-charger",
    },
    {},
};

static const struct dev_pm_ops sm5424_pm_ops = {
    .suspend = sm5424_suspend,
    .suspend_noirq = sm5424_suspend_noirq,
    .resume = sm5424_resume,
};

static const struct i2c_device_id sm5424_charger_id[] = {
    {"sm5424-charger", 0},
    {},
};
MODULE_DEVICE_TABLE(i2c, sm5424_charger_id);

static struct i2c_driver sm5424_charger_driver = {
    .driver =
        {
            .name = "sm5424-charger",
            .owner = THIS_MODULE,
            .of_match_table = sm5424_match_table,
            .pm = &sm5424_pm_ops,
        },
    .probe = sm5424_charger_probe,
    .remove = sm5424_charger_remove,
    .id_table = sm5424_charger_id,
};

module_i2c_driver(sm5424_charger_driver);

static int sm5424_charger_init(void) {
  i2c_register_driver(THIS_MODULE, &sm5424_charger_driver);
  return 0;
}

static void sm5424_charger_exit(void) {
  i2c_del_driver(&sm5424_charger_driver);
}

module_init(sm5424_charger_init);
module_exit(sm5424_charger_exit);

MODULE_DESCRIPTION("sm5424 charger in parallel");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("i2c:sm5424-charger"); 
