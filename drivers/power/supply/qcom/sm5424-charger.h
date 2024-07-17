/*
 * SM5424 charger driver (.h)
 * ---------------------
 * Copyright (c) 2021, MeizuCustoms (Roman Rihter)
 */

#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/pm.h>
#include <linux/pm_wakeup.h>
#include <linux/power_supply.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/types.h>

#ifndef __SM5424_CHARGER_H
#define __SM5424_CHARGER_H

// Registers; from sm5424.c for MTK CPUs

#define SM5424_INT1 0x00
#define SM5424_INT2 0x01
#define SM5424_INT3 0x02
#define SM5424_INTMASK1 0x03
#define SM5424_INTMASK2 0x04
#define SM5424_INTMASK3 0x05
#define SM5424_STATUS1 0x06
#define SM5424_STATUS2 0x07
#define SM5424_STATUS3 0x08
#define SM5424_CNTL 0x09
#define SM5424_VBUSCNTL 0x0A
#define SM5424_CHGCNTL1 0x0B
#define SM5424_CHGCNTL2 0x0C
#define SM5424_CHGCNTL3 0x0D
#define SM5424_CHGCNTL4 0x0E
#define SM5424_CHGCNTL5 0x0F
#define SM5424_CHGCNTL6 0x10
#define SM5424_DEVICEID 0x37

// Masks

#define SM5424_CNTL_CHGEN_MASK 0x1
#define SM5424_CNTL_OTGEN_MASK 0x4
#define SM5424_CNTL_RECHGREG_MASK 0x2

#define SM5424_VBUSCNTL_VBUSLIMIT_MASK 0x7F

#define SM5424_CHGCNTL2_FASTCHG_MASK 0x3F
#define SM5424_CHGCNTL2_RECHG_MASK 0x40

#define SM5424_CHGCNTL3_BATREG_MASK 0x3F

#define SM5424_CHGCNTL4_FREQSEL_MASK 0x30
#define SM5424_CHGCNTL4_TOPOFF_MASK 0xF

#define SM5424_CHGCNTL5_OTGCURRENT_MASK 0xC

#define SM5424_CHGCNTL6_VOTG_MASK 0x3

enum reason {
  USER = BIT(0),
  THERMAL = BIT(1),
  CURRENT = BIT(2),
  SOC = BIT(3),
  PARALLEL = BIT(4),
  ITERM = BIT(5),
  OTG = BIT(6),
};

struct sm5424_charger {
  struct device *dev;
  struct i2c_client *client;
  struct power_supply *parallel_psy;
  struct power_supply_desc	parallel_psy_d;
  struct mutex fcc_lock;
  struct dentry *debug_root;

  int battchg_disabled_status;
  int usb_suspended_status;

  bool is_otg_boost;
  bool is_slave;
  bool recharge_disabled;
  bool parallel_charger_present;
  bool check_parallel;
  int version;
  int chg_present;
  int irq;

  int32_t target_fastchg_current_max_ma;
  int32_t fastchg_current_max_ma;

  int32_t irq_gpio;
  int32_t vfloat_mv;
  int32_t recharge_mv;
  int32_t iterm_ma;
  int32_t switch_freq;
};

enum power_supply_property sm5424_psy_properties[7] = {
    POWER_SUPPLY_PROP_CHARGING_ENABLED,
    POWER_SUPPLY_PROP_STATUS,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_CURRENT_MAX,
    POWER_SUPPLY_PROP_VOLTAGE_MAX,
    POWER_SUPPLY_PROP_INPUT_CURRENT_LIMITED,
    POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX};

// Mark debug info as warnings?
//#define SM5424_DEBUG_HIGH_LEVEL

#ifdef SM5424_DEBUG_HIGH_LEVEL
#define sm5424_dbg(fmt, args...) pr_warn("sm5424: %s: " fmt, __func__, ##args);
#else
#define sm5424_dbg(fmt, args...) pr_debug("sm5424: %s: " fmt, __func__, ##args);
#endif

#endif 
