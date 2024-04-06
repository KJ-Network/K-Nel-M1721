/* Copyright (c) 2017, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define pr_fmt(fmt) "%s:%d " fmt, __func__, __LINE__

#include "msm_led_flash.h"
#include "msm_camera_dt_util.h"
#include "msm_cci.h"
#include <linux/module.h>

#undef CDBG
#define CDBG(fmt, args...) pr_err(fmt, ##args)

static struct v4l2_file_operations msm_led_flash_v4l2_subdev_fops;

static long msm_led_flash_subdev_ioctl(struct v4l2_subdev *sd, unsigned int cmd,
                                       void *arg) {
  struct msm_led_flash_ctrl_t *fctrl = NULL;
  void *argp = (void *)arg;

  if (!sd) {
    pr_err("sd NULL\n");
    return -EINVAL;
  }
  fctrl = v4l2_get_subdevdata(sd);
  if (!fctrl) {
    pr_err("fctrl NULL\n");
    return -EINVAL;
  }
  switch (cmd) {
  case VIDIOC_MSM_SENSOR_GET_SUBDEV_ID:
    return fctrl->func_tbl->flash_get_subdev_id(fctrl, argp);
  case VIDIOC_MSM_FLASH_CFG:
  case VIDIOC_MSM_FLASH_LED_DATA_CFG:
    return fctrl->func_tbl->flash_led_config(fctrl, argp);
  case MSM_SD_NOTIFY_FREEZE:
    return 0;
  case MSM_SD_SHUTDOWN:
    return fctrl->func_tbl->flash_led_release(fctrl);
  default:
    pr_err_ratelimited("invalid cmd 0x%02x\n", cmd);
    return -ENOIOCTLCMD;
  }
}

static struct v4l2_subdev_core_ops msm_flash_subdev_core_ops = {
    .ioctl = msm_led_flash_subdev_ioctl,
};

static struct v4l2_subdev_ops msm_flash_subdev_ops = {
    .core = &msm_flash_subdev_core_ops,
};

static const struct v4l2_subdev_internal_ops msm_flash_internal_ops;

#ifdef CONFIG_COMPAT
static long msm_led_flash_subdev_do_ioctl(struct file *file, unsigned int cmd,
                                          void *arg) {
  int32_t i = 0;
  int32_t rc = 0;
  struct video_device *vdev;
  struct v4l2_subdev *sd;
  struct msm_flash_cfg_data_t32 *u32;
  struct msm_flash_cfg_data_t flash_data;
  struct msm_flash_init_info_t32 flash_init_info32;
  struct msm_flash_init_info_t flash_init_info;

  CDBG("Enter");

  if (!file || !arg) {
    pr_err("%s:failed NULL parameter\n", __func__);
    return -EINVAL;
  }
  vdev = video_devdata(file);
  sd = vdev_to_v4l2_subdev(vdev);
  u32 = (struct msm_flash_cfg_data_t32 *)arg;

  switch (cmd) {
  case VIDIOC_MSM_FLASH_CFG32:
    flash_data.cfg_type = u32->cfg_type;
    for (i = 0; i < MAX_LED_TRIGGERS; i++) {
      flash_data.flash_current[i] = u32->flash_current[i];
      flash_data.flash_duration[i] = u32->flash_duration[i];
    }
    cmd = VIDIOC_MSM_FLASH_CFG;
    switch (flash_data.cfg_type) {
    case CFG_FLASH_OFF:
    case CFG_FLASH_LOW:
    case CFG_FLASH_HIGH:
      flash_data.cfg.settings = compat_ptr(u32->cfg.settings);
      break;
    case CFG_FLASH_INIT:
      flash_data.cfg.flash_init_info = &flash_init_info;
      if (copy_from_user(&flash_init_info32,
                         (void *)compat_ptr(u32->cfg.flash_init_info),
                         sizeof(struct msm_flash_init_info_t32))) {
        pr_err("%s copy_from_user failed %d\n", __func__, __LINE__);
        return -EFAULT;
      }
      flash_init_info.flash_driver_type = flash_init_info32.flash_driver_type;
      flash_init_info.slave_addr = flash_init_info32.slave_addr;
      flash_init_info.i2c_freq_mode = flash_init_info32.i2c_freq_mode;
      flash_init_info.settings = compat_ptr(flash_init_info32.settings);
      flash_init_info.power_setting_array =
          compat_ptr(flash_init_info32.power_setting_array);
      break;
    default:
      break;
    }
    break;
  case VIDIOC_MSM_FLASH_CFG:
    pr_err("invalid cmd 0x%x received\n", cmd);
    return -EINVAL;
  default:
    return msm_led_flash_subdev_ioctl(sd, cmd, arg);
  }

  rc = msm_led_flash_subdev_ioctl(sd, cmd, &flash_data);
  for (i = 0; i < MAX_LED_TRIGGERS; i++) {
    u32->flash_current[i] = flash_data.flash_current[i];
    u32->flash_duration[i] = flash_data.flash_duration[i];
  }
  CDBG("Exit");
  return rc;
}

static long msm_led_flash_subdev_fops_ioctl(struct file *file, unsigned int cmd,
                                            unsigned long arg) {
  return video_usercopy(file, cmd, arg, msm_led_flash_subdev_do_ioctl);
}
#endif

int32_t msm_led_flash_create_v4lsubdev(struct platform_device *pdev,
                                       void *data) {
  struct msm_led_flash_ctrl_t *fctrl = (struct msm_led_flash_ctrl_t *)data;
  CDBG("Enter\n");

  if (!fctrl) {
    pr_err("fctrl NULL\n");
    return -EINVAL;
  }

  /* Initialize sub device */
  v4l2_subdev_init(&fctrl->msm_sd.sd, &msm_flash_subdev_ops);
  v4l2_set_subdevdata(&fctrl->msm_sd.sd, fctrl);

  fctrl->pdev = pdev;
  fctrl->msm_sd.sd.internal_ops = &msm_flash_internal_ops;
  fctrl->msm_sd.sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
  snprintf(fctrl->msm_sd.sd.name, ARRAY_SIZE(fctrl->msm_sd.sd.name),
           "msm_flash");
  media_entity_pads_init(&fctrl->msm_sd.sd.entity, 0, NULL);
  fctrl->msm_sd.sd.entity.group_id = MSM_CAMERA_SUBDEV_FLASH;
  fctrl->msm_sd.close_seq = MSM_SD_CLOSE_2ND_CATEGORY | 0x1;
  msm_sd_register(&fctrl->msm_sd);

  msm_led_flash_v4l2_subdev_fops = v4l2_subdev_fops;
#ifdef CONFIG_COMPAT
  msm_led_flash_v4l2_subdev_fops.compat_ioctl32 =
      msm_led_flash_v4l2_subdev_fops.unlocked_ioctl;
#endif
  fctrl->msm_sd.sd.devnode->fops = &msm_led_flash_v4l2_subdev_fops;
  CDBG("probe success\n");
  return 0;
}

int32_t msm_led_i2c_flash_create_v4lsubdev(void *data) {
  struct msm_led_flash_ctrl_t *fctrl = (struct msm_led_flash_ctrl_t *)data;
  CDBG("Enter\n");

  if (!fctrl) {
    pr_err("fctrl NULL\n");
    return -EINVAL;
  }

  /* Initialize sub device */
  v4l2_subdev_init(&fctrl->msm_sd.sd, &msm_flash_subdev_ops);
  v4l2_set_subdevdata(&fctrl->msm_sd.sd, fctrl);

  fctrl->msm_sd.sd.internal_ops = &msm_flash_internal_ops;
  fctrl->msm_sd.sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
  snprintf(fctrl->msm_sd.sd.name, ARRAY_SIZE(fctrl->msm_sd.sd.name),
           "msm_flash");
  media_entity_pads_init(&fctrl->msm_sd.sd.entity, 0, NULL);
  fctrl->msm_sd.sd.entity.group_id = MSM_CAMERA_SUBDEV_LED_FLASH;
  msm_sd_register(&fctrl->msm_sd);

  msm_led_flash_v4l2_subdev_fops = v4l2_subdev_fops;
#ifdef CONFIG_COMPAT
  msm_led_flash_v4l2_subdev_fops.compat_ioctl32 =
      msm_led_flash_subdev_fops_ioctl;
#endif
  fctrl->msm_sd.sd.devnode->fops = &msm_led_flash_v4l2_subdev_fops;

  CDBG("probe success\n");
  return 0;
}
