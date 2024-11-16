/*
 * Meizu hardware info - Low-level kernel driver
 *
 * Copyright (C) 2020 MeizuCustoms enthusiasts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/proc_fs.h>

#define MZHW_USED_FOR_CAMERA

#include <media/meizu_hw.h>

struct class *mzhw_class;
bool mback_disable = false;

static struct proc_dir_entry *ent;
 
static ssize_t disable_mback_write(struct file *file, const char __user *buf,
                       size_t count, loff_t *ppos) 
{
	char value[2];
  int ret, i, c;

  ret = copy_from_user(&value, buf, 2);
  if (ret)
    return ret;

  ret = sscanf(value, "%u", &i);
  if (ret != 1 || i > 1)
    return ret;

  mback_disable = i;

  c = strlen(buf);
  *ppos = c;
	return c;
}
 
static ssize_t disable_mback_read(struct file *file, char __user *buf,
                      size_t count, loff_t *ppos) 
{
	char value[2];
  int len = 0, ret = 0;

  len = sprintf(value, "%c\n", (char)mback_disable ? '1' : '0');

  ret = copy_to_user(buf, value, len);
  if (ret)
    return ret;

  *ppos = len;
	return len;
}
 
static struct file_operations disable_mback_ops = 
{
	.owner = THIS_MODULE,
	.read = disable_mback_read,
	.write = disable_mback_write,
};

int meizu_sensor_parse_id(struct meizu_camera_data *camera,
                          struct msm_sensor_ctrl_t *s_ctrl) {
  unsigned int sensor_id = 0;
  const char *sensor_name;
  uint16_t camera_revision = 0;
  int rc = 0;

  if (!camera || !s_ctrl) {
    mz_err("Some arguments were not found\n");
    return -EINVAL;
  }

  sensor_id = s_ctrl->sensordata->slave_info->sensor_id;
  sensor_name = s_ctrl->sensordata->sensor_name;

  switch (sensor_id) {
  case 0x362:
    strcpy(camera->back.name, "imx362");
    strcpy(camera->back.lens, "qtech");
    camera->back.id = sensor_id;
    break;
  case 0x3108:
    s_ctrl->sensor_i2c_client->cci_client->sid = 80;
    rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_read(
        s_ctrl->sensor_i2c_client, 1, &camera_revision, 1);
    if (rc) {
      mz_err("Failed to read camera id (s5k3p8sp03)\n");
      return rc;
    }

    mz_info("s5k3p8sp03 sid = %d\n",
            s_ctrl->sensor_i2c_client->cci_client->sid);

    if (camera_revision != 6)
      strcpy(camera->front.lens, "shengtai");
    else
      strcpy(camera->front.lens, "qtech");

    strcpy(camera->front.name, "s5k3p8sp03");
    camera->front.id = sensor_id;
    break;
  case 0x20c7:
    s_ctrl->sensor_i2c_client->cci_client->sid = 80;
    rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_read(
        s_ctrl->sensor_i2c_client, 1, &camera_revision, 1);
    if (rc) {
      s_ctrl->sensor_i2c_client->cci_client->sid = 81;
      rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_read(
          s_ctrl->sensor_i2c_client, 1, &camera_revision, 1);
      if (rc) {
        mz_err("Failed to read camera id (s5k2l7)");
        return rc;
      }
    }

    mz_info("s5k2l7 sid = %d\n", s_ctrl->sensor_i2c_client->cci_client->sid);

    switch (camera_revision) {
    case 1:
      strcpy(camera->back.lens, "sunny");
      break;
    case 3:
      strcpy(camera->back.lens, "ofilm");
      break;
    case 7:
      strcpy(camera->back.lens, "primax");
      break;
    default:
      mz_err("Failed to check lens vendor (s5k2l7)\n");
      return -EINVAL;
    }

    strcpy(camera->back.name, "s5k2l7");
    camera->back.id = sensor_id;
    break;
  case 0x4e8:
    if (!strcmp(camera->back.name, "imx362") &&
        !strcmp(camera->back.lens, "qtech"))
      strcpy(camera->bokeh.lens, "qtech");
    else
      strcpy(camera->bokeh.lens, "primax");

    strcpy(camera->bokeh.name, "s5k4e8");
    camera->bokeh.id = sensor_id;
    break;
  case 0x501:
    strcpy(camera->bokeh.name, "c5490");
    strcpy(camera->bokeh.lens, "hlt");
    camera->bokeh.id = sensor_id;
    break;
  case 0x569:
    strcpy(camera->bokeh.name, "ov5695");
    strcpy(camera->bokeh.lens, "hlt");
    camera->bokeh.id = sensor_id;
    break;
  default:
    mz_err("Unknown camera id!\n");
    return -EINVAL;
  }

  if (strcmp(sensor_name, camera->back.name) &&
      strcmp(sensor_name, camera->front.name) &&
      strcmp(sensor_name, camera->bokeh.name)) {
    mz_err("Sensor name mismatch!\n");
    return -EINVAL;
  }

  return 0;
}

int meizu_camera_print_data(struct meizu_camera_data *camera) {
  if (!camera) {
    mz_err("Camera structure not found.\n");
    return -EINVAL;
  }

  if (camera->back.id) {
    mz_info(
        "Back camera summary: you have sensor %s with %s lens, id is 0x%02x.\n",
        camera->back.name, camera->back.lens, camera->back.id);
  }
  if (camera->front.id) {
    mz_info("Front camera summary: you have sensor %s with %s lens, id is "
            "0x%02x.\n",
            camera->front.name, camera->front.lens, camera->front.id);
  }
  if (camera->bokeh.id) {
    mz_info("Bokeh camera summary: you have sensor %s with %s lens, id is "
            "0x%02x.\n",
            camera->bokeh.name, camera->bokeh.lens, camera->bokeh.id);
  }

  return 0;
}

int meizu_hw_class_init(void) {
  if (!mzhw_class || !IS_ERR(mzhw_class)) {
    mzhw_class = class_create(THIS_MODULE, "mzhw");
    if (IS_ERR(mzhw_class)) {
      mz_err("Failed to register mzhw class.\n");
      return PTR_ERR(mzhw_class);
    }
  }

  return 0;
}

static int __init meizu_hw_init(void) {
  ent = proc_create("mback_disable", 0666, NULL, &disable_mback_ops);
  mz_info("inited.\n");
  return 0;
}

static void meizu_hw_exit(void) {
  proc_remove(ent);
}

module_init(meizu_hw_init);
module_exit(meizu_hw_exit);
