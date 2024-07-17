/*
 * Copyright (C) 2020 MeizuCustoms enthusiasts
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

#include "msm_led_flash.h"

#define aw_info(fmt, args...) pr_info("aw36413: %s: " fmt, __func__, ##args);
#define aw_warn(fmt, args...) pr_warn("aw36413: %s: " fmt, __func__, ##args);
#define aw_err(fmt, args...) pr_err("aw36413: %s: " fmt, __func__, ##args);

/*
 * All values were taken from
 * https://datasheet.lcsc.com/szlcsc/Shanghai-Awinic-Tech-AW36413CSR_C252447.pdf
 */

#define AW36413_TIMING_COUNT 22 // Ramp time: 1ms | Flash time: 1000ms
#define AW36413_HWEN_DELAY 5

struct aw36413_effects_data {
  int device;
  unsigned int reg;
  unsigned int val;
  int delay;
};

/*
 *  Main aw36413 data structure
 */
struct aw36413_cfg {
  // Devices
  struct i2c_adapter *a1; // first aw36413 i2c adapter
  struct i2c_adapter *a2; // second aw36413 i2c adapter

  // States
  int ready;   // ready to power on LEDs?
  int probed;  // is i2c_probe succeed?
  int hwenpwr; // is HWEN powered?
  int proc;    // proc state

  // Info
  int vendor; // Vendor ID

  // GPIOs
  unsigned int hwen1;  // HWEN for first aw36413
  unsigned int hwen2;  // HWEN for second aw36413
  unsigned int strobe; // Strobe pin

  // Classdev
  struct device *dev;
  struct class *class;
  struct cdev cdev;
  dev_t devt;
  const struct file_operations fops;

  // I2C data
  const struct i2c_device_id *id; // aw36413 i2c id
};

/*
 *  aw36413 GPIO states
 */
enum aw36413_gpio {
  AW36413_HWEN_OFF = 0,
  AW36413_HWEN_ON = 1,
};

/*
 *  Device codes for aw36413_read_reg and aw36413_write_reg
 */
enum aw36413_ops {
  AW36413_FIRST = 1,
  AW36413_SECOND,
  AW36413_BOTH,
};

/*
 *  I2C registers for aw36413
 */
enum aw36413_registers {
  REG_AW36413_CHIP_ID = 0x0,    // Chip ID Register
  REG_AW36413_ENABLE = 0x1,     // Enable Register
  REG_AW36413_IVFM = 0x2,       // IVFM Register
  REG_AW36413_LED1_FLASH = 0x3, // LED1 Flash Brightness Register
  REG_AW36413_LED2_FLASH = 0x4, // LED2 Flash Brightness Register
  REG_AW36413_LED1_TORCH = 0x5, // LED1 Torch Brightness Register
  REG_AW36413_LED2_TORCH = 0x6, // LED2 Torch Brightness Register
  REG_AW36413_BOOST = 0x7,      // Boost Configuration Register
  REG_AW36413_TIMING = 0x8,     // Timing Configuration Register
  REG_AW36413_TEMP = 0x9,       // Temp Register
  REG_AW36413_FLAGS1 = 0xa,     // Flags1 Register
  REG_AW36413_FLAGS2 = 0xb,     // Flags2 Register
  REG_AW36413_DEVICE_ID = 0xc,  // Device ID Register
  REG_AW36413_LAST_FLASH = 0xd, // Last Flash Register
  REG_AW36413_IND_CUR = 0x39,   // Indicator Current Register
};