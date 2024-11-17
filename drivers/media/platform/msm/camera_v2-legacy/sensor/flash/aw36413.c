/*
 * Copyright (C) 2020-2021 MeizuCustoms enthusiasts
 *
 * Part of the MeizuSucks project
 * v2.0-legacy (Sep 2020)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "aw36413.h"
#include "../../common/msm_camera_io_util.h"
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/export.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/stddef.h>
#include <linux/types.h>
#include <media/meizu_hw.h>

static struct msm_led_flash_ctrl_t fctrl;

static const struct i2c_device_id aw36413_i2c_id[] = {
    {"awinic,aw36413", (kernel_ulong_t)&fctrl},
    {},
};
static const struct of_device_id aw36413_i2c_trigger_dt_match[] = {
    {.compatible = "awinic,aw36413"},
    {},
};
MODULE_DEVICE_TABLE(of, aw36413_trigger_dt_match);

static struct aw36413_cfg *aw36413 = NULL;

static int aw36413_reg_write(int device, unsigned int reg, unsigned int val) {
  unsigned int ret;

  if (!fctrl.flash_i2c_client->i2c_func_tbl) {
    aw_err("I2C functions are not defined!\n");
    return -EINVAL;
  }

  switch (device) {
  case AW36413_FIRST:
    fctrl.flash_i2c_client->client->adapter = aw36413->a1;
    ret = fctrl.flash_i2c_client->i2c_func_tbl->i2c_write(
        fctrl.flash_i2c_client, reg, val, MSM_CAMERA_I2C_BYTE_DATA);
    break;
  case AW36413_SECOND:
    fctrl.flash_i2c_client->client->adapter = aw36413->a2;
    ret = fctrl.flash_i2c_client->i2c_func_tbl->i2c_write(
        fctrl.flash_i2c_client, reg, val, MSM_CAMERA_I2C_BYTE_DATA);
    break;
  case AW36413_BOTH:
    fctrl.flash_i2c_client->client->adapter = aw36413->a1;
    ret = fctrl.flash_i2c_client->i2c_func_tbl->i2c_write(
        fctrl.flash_i2c_client, reg, val, MSM_CAMERA_I2C_BYTE_DATA);
    if (ret < 0) {
      aw_err("error: %d, device: %d, reg: 0x%02x, val: 0x%02x\n", ret, device,
             reg, val);
      return ret;
    }
    fctrl.flash_i2c_client->client->adapter = aw36413->a2;
    ret = fctrl.flash_i2c_client->i2c_func_tbl->i2c_write(
        fctrl.flash_i2c_client, reg, val, MSM_CAMERA_I2C_BYTE_DATA);
    break;
  default:
    aw_err("Invalid device %d. Please use aw36413_ops values.\n", device);
    return -EINVAL;
    break;
  }

  if (ret < 0) {
    aw_err("error: %d, device: %d, reg: 0x%02x, val: 0x%02x\n", ret, device,
           reg, val);
    return ret;
  } else {
    aw_info("done %d, device: %d, reg: 0x%02x, val: 0x%02x\n", ret, device, reg,
            val);
  }

  return 0;
}

static unsigned int aw36413_reg_read(int device, unsigned int reg) {
  unsigned int ret;
  uint8_t val;

  if (!fctrl.flash_i2c_client->i2c_func_tbl) {
    aw_err("I2C functions are not defined!\n");
    return -EINVAL;
  }

  switch (device) {
  case AW36413_FIRST:
    fctrl.flash_i2c_client->client->adapter = aw36413->a1;
    ret = fctrl.flash_i2c_client->i2c_func_tbl->i2c_read_seq(
        fctrl.flash_i2c_client, reg, &val, MSM_CAMERA_I2C_BYTE_DATA);
    break;
  case AW36413_SECOND:
    fctrl.flash_i2c_client->client->adapter = aw36413->a2;
    ret = fctrl.flash_i2c_client->i2c_func_tbl->i2c_read_seq(
        fctrl.flash_i2c_client, reg, &val, MSM_CAMERA_I2C_BYTE_DATA);
    break;
  case AW36413_BOTH:
    aw_err("both mode not supported\n");
    break;
  default:
    aw_err("Invalid device %d. Please use aw36413_ops values.\n", device);
    return -EINVAL;
    break;
  }

  if (ret < 0) {
    aw_err("error: %d, device: %d, reg: 0x%02x, val: 0x%02x\n", ret, device,
           reg, val);
    return ret;
  } else {
    aw_info("done %d, device: %d, reg: 0x%02x, val: 0x%02x\n", ret, device, reg,
            val);
  }

  return val;
}

static void aw36413_hwen(int device, int state) {
  switch (device) {
  case AW36413_FIRST:
    if (gpio_is_valid(aw36413->hwen1)) {
      gpio_direction_output(aw36413->hwen1, state);
    } else {
      aw_err("HWEN1 invalid!\n");
      return;
    }
    break;
  case AW36413_SECOND:
    if (gpio_is_valid(aw36413->hwen1)) {
      gpio_direction_output(aw36413->hwen2, state);
    } else {
      aw_err("HWEN2 invalid!\n");
      return;
    }
    break;
  case AW36413_BOTH:
    if (gpio_is_valid(aw36413->hwen1)) {
      gpio_direction_output(aw36413->hwen1, state);
    } else {
      aw_err("HWEN1 invalid!\n");
      return;
    }
    if (gpio_is_valid(aw36413->hwen2)) {
      gpio_direction_output(aw36413->hwen2, state);
    } else {
      aw_err("HWEN1 invalid!\n");
      return;
    }
    break;
  default:
    aw_err("Invalid device %d. Please use aw36413_ops values.\n", device);
    return;
    break;
  }

  aw36413->hwenpwr = 1;
  if (state == AW36413_HWEN_OFF)
    aw36413->hwenpwr = 0;

  aw_info("Success, device: %d, state: %d\n", device, state);
  return;
}

static void aw36413_get_vendor_id(void) {
  int devid;
  aw36413_hwen(AW36413_FIRST, AW36413_HWEN_OFF);
  mdelay(10);
  aw36413_hwen(AW36413_FIRST, AW36413_HWEN_ON);
  mdelay(10);
  aw36413->vendor = 0;
  fctrl.flash_i2c_client->client->addr = 0xc6;
  devid = aw36413_reg_read(AW36413_FIRST, REG_AW36413_DEVICE_ID);
  aw_info("device id: 0x%02x\n", devid);
  if (aw36413_reg_read(AW36413_FIRST, REG_AW36413_DEVICE_ID) != 0x1c) {
    fctrl.flash_i2c_client->client->addr = 0xd6;
    aw36413->vendor = 1;
  }
  aw_info("done\n");
}

static int aw36413_request_gpio(void) {
  int ret;

  if (gpio_is_valid(aw36413->hwen1)) {
    ret = gpio_request(aw36413->hwen1, "aw36413_hwen1");
    if (ret < 0) {
      aw_err("Failed to request HWEN1: %d\n", ret);
      goto hwen1;
    }
  }

  if (gpio_is_valid(aw36413->hwen2)) {
    ret = gpio_request(aw36413->hwen2, "aw36413_hwen2");
    if (ret < 0) {
      aw_err("Failed to request HWEN2: %d\n", ret);
      goto hwen2;
    }
  }

  if (gpio_is_valid(aw36413->strobe)) {
    ret = gpio_request(aw36413->strobe, "aw36413_strobe");
    if (ret < 0) {
      aw_err("Failed to request STROBE: %d\n", ret);
      goto strobe;
    }
  }

  return 0;
strobe:
  gpio_free(aw36413->hwen2);
hwen2:
  gpio_free(aw36413->hwen1);
hwen1:
  return ret;
}

static int msm_flash_aw36413_i2c_probe(struct i2c_client *client,
                                       const struct i2c_device_id *id) {
  int ret;

  aw36413 = kzalloc(sizeof(struct aw36413_cfg *), GFP_KERNEL);
  aw36413->id = aw36413_i2c_id;
  aw36413->hwen1 = 95;
  aw36413->hwen2 = 93;
  aw36413->strobe = 96;
  aw36413->a1 = i2c_get_adapter(6);
  aw36413->a2 = i2c_get_adapter(5);
  client->adapter = aw36413->a1;

  if (!id)
    id = aw36413_i2c_id;

  if (!aw36413->a1) {
    aw_err("no a1 found.\n");
    return -EINVAL;
  }
  if (!aw36413->a2) {
    aw_err("no a2 found.\n");
    return -EINVAL;
  }

  if (!i2c_check_functionality(aw36413->a1, I2C_FUNC_I2C)) {
    aw_err("i2c adapter 1 is not functional\n");
    return -EINVAL;
  }

  if (!i2c_check_functionality(aw36413->a2, I2C_FUNC_I2C)) {
    aw_err("i2c adapter 2 is not functional\n");
    return -EINVAL;
  }

  msm_flash_i2c_probe(client, id);

  ret = aw36413_request_gpio();
  if (ret) {
    aw_err("GPIO request error\n");
    return ret;
  }

  if (fctrl.pinctrl_info.use_pinctrl != false) {
    aw_info("set pinctrl state to active\n");
    ret = pinctrl_select_state(fctrl.pinctrl_info.pinctrl,
                               fctrl.pinctrl_info.gpio_state_active);
    if (ret) {
      aw_err("error during pinctrl state changing\n");
    }
  }

  aw36413_get_vendor_id();

  ret = msm_i2c_torch_create_classdev(&client->dev, NULL);
  if (ret) {
    pr_err("%s: failed to create classdev %d\n", __func__, __LINE__);
    return ret;
  }

success:
  aw_info("done\n");
  aw36413->probed = 1;
  return 0;
}

static int msm_flash_aw36413_i2c_remove(struct i2c_client *client) {
  int ret = 0;

  aw_info("enter\n");

  if (fctrl.pinctrl_info.use_pinctrl != false) {
    aw_info("set pinctrl state to suspend\n");
    ret = pinctrl_select_state(fctrl.pinctrl_info.pinctrl,
                               fctrl.pinctrl_info.gpio_state_suspend);
    if (ret) {
      aw_err("error during pinctrl state changing\n");
      return ret;
    }
  }

  gpio_free(aw36413->hwen1);
  gpio_free(aw36413->hwen2);
  gpio_free(aw36413->strobe);
  aw_info("done\n");
  aw36413->probed = 0;
  return ret;
}

static int msm_flash_aw36413_led_init(struct msm_led_flash_ctrl_t *fctrl) {
  aw_info("enter\n");
  aw36413_hwen(AW36413_BOTH, AW36413_HWEN_OFF);
  mdelay(AW36413_HWEN_DELAY);
  aw36413_hwen(AW36413_BOTH, AW36413_HWEN_ON);
  mdelay(AW36413_HWEN_DELAY);
  aw36413_reg_write(AW36413_BOTH, REG_AW36413_ENABLE, 0);
  aw36413->ready = 1;
  return 0;
}

static int msm_flash_aw36413_led_release(struct msm_led_flash_ctrl_t *fctrl) {
  aw_info("enter\n");
  if (!fctrl || !fctrl->flashdata) {
    aw_err("fctrl not found\n");
    return -EINVAL;
  }

  aw36413_hwen(AW36413_BOTH, AW36413_HWEN_OFF);
  mdelay(10);
  aw36413->ready = 0;
  return 0;
}

static int msm_flash_aw36413_led_off(struct msm_led_flash_ctrl_t *fctrl) {
  int ret;

  aw_info("enter\n");
  if (!fctrl || !fctrl->flashdata) {
    aw_err("fctrl not found\n");
    return -EINVAL;
  }

  if (aw36413->hwenpwr) {
    aw36413_hwen(AW36413_BOTH, AW36413_HWEN_OFF);
    mdelay(AW36413_HWEN_DELAY); // HWEN	delay
  }
  aw36413_hwen(AW36413_BOTH, AW36413_HWEN_ON);
  mdelay(AW36413_HWEN_DELAY); // HWEN	delay

  ret = aw36413_reg_write(AW36413_BOTH, REG_AW36413_ENABLE, 0);
  if (ret) {
    aw_err("failed to write reg\n");
    return ret;
  }

  return 0;
}

static int msm_flash_aw36413_led_high(struct msm_led_flash_ctrl_t *fctrl) {
  int brightness[2];
  int enable = 15;

  aw_info("enter\n");
  if (!fctrl || !fctrl->flashdata) {
    aw_err("fctrl not found\n");
    return -EINVAL;
  }

  if (fctrl->cci_i2c_master) {
    aw_info("cci_master = %d\n", fctrl->cci_i2c_master);
  }

  aw_info("br1 = %d, br2 = %d\n", fctrl->flash_op_current[0],
          fctrl->flash_op_current[1]);

  if (fctrl->flash_op_current[0] > 1500)
    brightness[0] = 63;
  else
    brightness[0] = ((100 * fctrl->flash_op_current[0] + 1170) / 2344) - 1;

  if (fctrl->flash_op_current[1] > 1500)
    brightness[0] = 63;
  else
    brightness[1] = ((100 * fctrl->flash_op_current[1] + 1170) / 2344) - 1;

  aw_info("registers br1 = %d, br2 = %d\n", brightness[0], brightness[1]);

  if (!brightness[0])
    enable = 13;
  if (!brightness[1])
    enable = 14;
  if (!brightness[1] && !brightness[0])
    enable = 0;

  if (!brightness[0] || !brightness[1]) {
    aw_err("Brightness error: %d[0] and %d[1]", brightness[0], brightness[1]);
  }

  if (aw36413->ready) {
    if (aw36413->hwenpwr) {
      aw36413_hwen(AW36413_BOTH, AW36413_HWEN_OFF);
      mdelay(AW36413_HWEN_DELAY); // HWEN	delay
    }
    aw36413_hwen(AW36413_BOTH, AW36413_HWEN_ON);
    mdelay(AW36413_HWEN_DELAY); // HWEN	delay
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_LED1_FLASH, brightness[0]);
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_LED2_FLASH, brightness[1]);
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_TIMING, AW36413_TIMING_COUNT);
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_ENABLE, enable);
  } else {
    aw_info("aw36413 not ready\n");
  }
  return 0;
}

static int msm_flash_aw36413_led_low(struct msm_led_flash_ctrl_t *fctrl) {
  int countVar[2], brightness[2];
  int enable = 11;

  aw_info("enter\n");
  if (!fctrl || !fctrl->flashdata) {
    aw_err("fctrl not found\n");
    return -EINVAL;
  }

  if (fctrl->cci_i2c_master) {
    aw_info("cci_master = %d\n", fctrl->cci_i2c_master);
  }

  aw_info("br1 = %d, br2 = %d\n", fctrl->flash_op_current[0],
          fctrl->flash_op_current[1]);

  if (aw36413->vendor == 1) {
    countVar[0] = 560;
    countVar[1] = 280;
  } else {
    countVar[0] = 582;
    countVar[1] = 291;
  }

  if (fctrl->flash_op_current[0] > 300)
    brightness[0] = ((30000 + countVar[1]) / countVar[0]) - 1;
  else
    brightness[0] =
        ((100 * fctrl->flash_op_current[0] + countVar[1]) / countVar[0]) - 1;

  if (fctrl->flash_op_current[1] > 300)
    brightness[1] = ((30000 + countVar[1]) / countVar[0]) - 1;
  else
    brightness[1] =
        ((100 * fctrl->flash_op_current[1] + countVar[1]) / countVar[0]) - 1;

  aw_info("registers br1 = %d, br2 = %d\n", brightness[0], brightness[1]);

  if (!brightness[0])
    enable = 9;
  if (!brightness[1])
    enable = 10;
  if (!brightness[1] && !brightness[0])
    enable = 0;

  if (!brightness[0] || !brightness[1]) {
    aw_err("Brightness error: %d[0] and %d[1]", brightness[0], brightness[1]);
  }

  if (aw36413->ready) {
    if (aw36413->hwenpwr) {
      aw36413_hwen(AW36413_BOTH, AW36413_HWEN_OFF);
      mdelay(AW36413_HWEN_DELAY); // HWEN	delay
    }
    aw36413_hwen(AW36413_BOTH, AW36413_HWEN_ON);
    mdelay(AW36413_HWEN_DELAY); // HWEN	delay
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_LED1_TORCH, brightness[0]);
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_LED2_TORCH, brightness[1]);
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_TIMING, AW36413_TIMING_COUNT);
    aw36413_reg_write(AW36413_BOTH, REG_AW36413_ENABLE, enable);
  } else {
    aw_info("aw36413 not ready\n");
  }
  return 0;
}

static struct msm_flash_fn_t aw36413_func_tbl = {
    .flash_get_subdev_id = msm_led_i2c_trigger_get_subdev_id,
    .flash_led_config = msm_flash_i2c_trigger_config,
    .flash_led_init = msm_flash_aw36413_led_init,
    .flash_led_release = msm_flash_aw36413_led_release,
    .flash_led_off = msm_flash_aw36413_led_off,
    .flash_led_low = msm_flash_aw36413_led_low,
    .flash_led_high = msm_flash_aw36413_led_high,
};

static struct msm_camera_i2c_client aw36413_i2c_client = {
    .addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
};

static struct msm_led_flash_ctrl_t fctrl = {
    .flash_i2c_client = &aw36413_i2c_client,
    .func_tbl = &aw36413_func_tbl,
    .flash_device_type = MSM_CAMERA_I2C_DEVICE,
    .cci_i2c_master = MASTER_0,
    .led_state = MSM_CAMERA_LED_OFF,
};

static struct i2c_driver aw36413_i2c_driver = {
    .id_table = aw36413_i2c_id,
    .probe = msm_flash_aw36413_i2c_probe,
    .remove = msm_flash_aw36413_i2c_remove,
    .driver =
        {
            .name = "awinic,aw36413",
            .owner = THIS_MODULE,
            .of_match_table = aw36413_i2c_trigger_dt_match,
        },
};

static int __init msm_flash_aw36413_init(void) {
  return i2c_add_driver(&aw36413_i2c_driver);
}

static void __exit msm_flash_aw36413_exit(void) {
  i2c_del_driver(&aw36413_i2c_driver);
}

late_initcall(msm_flash_aw36413_init);
module_exit(msm_flash_aw36413_exit);
MODULE_DESCRIPTION("AW36413 (x2) LED driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Roman Rihter <teledurak@msucks.space>");
