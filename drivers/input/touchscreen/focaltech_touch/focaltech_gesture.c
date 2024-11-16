/*
 *
 * FocalTech TouchScreen driver.
 *
 * Copyright (c) 2010-2017, Focaltech Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*****************************************************************************
 *
 * File Name: focaltech_gestrue.c
 *
 * Author: Focaltech Driver Team
 *
 * Created: 2016-08-08
 *
 * Abstract:
 *
 * Reference:
 *
 *****************************************************************************/

/*****************************************************************************
 * 1.Included header files
 *****************************************************************************/
#include "focaltech_core.h"
#if FTS_GESTURE_EN
/******************************************************************************
 * Private constant and macro definitions using #define
 *****************************************************************************/
#define FTS_GESTRUE_POINTS                      255
#define FTS_GESTRUE_POINTS_HEADER               8

#define GESTURE_SMALL_AREA      0x25    /* TP Coverage < 50% */
#define GESTURE_LARGE_AREA      0x26    /* TP Coverage > 50% */

/*****************************************************************************
 * Private enumerations, structures and unions using typedef
 *****************************************************************************/
/*
 * header        -   byte0:gesture id
 *                   byte1:pointnum
 *                   byte2~7:reserved
 * coordinate_x  -   All gesture point x coordinate
 * coordinate_y  -   All gesture point y coordinate
 * mode          -   1:enable gesture function(default)
 *               -   0:disable
 * active        -   1:enter into gesture(suspend)
 *                   0:gesture disable or resume
 */
struct fts_gesture_st {
	u8 header[FTS_GESTRUE_POINTS_HEADER];
	u16 coordinate_x[FTS_GESTRUE_POINTS];
	u16 coordinate_y[FTS_GESTRUE_POINTS];
	bool enabled;
};

/*****************************************************************************
 * Static variables
 *****************************************************************************/
static struct fts_gesture_st fts_gesture_data;

/*****************************************************************************
 * Global variable or extern global variabls/functions
 *****************************************************************************/

/*****************************************************************************
 * Static function prototypes
 *****************************************************************************/
static ssize_t fts_gesture_show(struct device *dev,
		struct device_attribute *attr, char *buf);
static ssize_t fts_gesture_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count);
static ssize_t fts_gesture_buf_show(struct device *dev,
		struct device_attribute *attr, char *buf);
static ssize_t fts_gesture_buf_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count);

/* sysfs gesture node
 *   read example: cat  fts_gesture_mode        ---read gesture mode
 *   write example:echo 01 > fts_gesture_mode   ---write gesture mode to 01
 *
 */
static DEVICE_ATTR(enable_dt2w, 0644,
		fts_gesture_show, fts_gesture_store);
/*
 *   read example: cat fts_gesture_buf        ---read gesture buf
 */
static DEVICE_ATTR(fts_gesture_buf, 0644,
		fts_gesture_buf_show, fts_gesture_buf_store);
static struct attribute *fts_gesture_mode_attrs[] = {
	&dev_attr_enable_dt2w.attr,
	&dev_attr_fts_gesture_buf.attr,
	NULL,
};

static struct attribute_group fts_gesture_group = {

	.attrs = fts_gesture_mode_attrs,
};

/************************************************************************
 * Name: fts_gesture_show
 *  Brief:
 *  Input: device, device attribute, char buf
 * Output:
 * Return:
 ***********************************************************************/
static ssize_t fts_gesture_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int count;
	u8 val;
	struct i2c_client *client = container_of(dev, struct i2c_client, dev);

	mutex_lock(&fts_input_dev->mutex);
	fts_i2c_read_reg(client, FTS_REG_GESTURE_EN, &val);
	count = snprintf(buf, PAGE_SIZE, "Gesture Mode: %s\n",
			fts_gesture_data.enabled ? "On" : "Off");
	count += snprintf(buf + count, PAGE_SIZE - count,
				"Reg(0xD0) = %d\n", val);
	mutex_unlock(&fts_input_dev->mutex);

	return count;
}

/************************************************************************
 * Name: fts_gesture_store
 *  Brief:
 *  Input: device, device attribute, char buf, char count
 * Output:
 * Return:
 ***********************************************************************/
static ssize_t fts_gesture_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	mutex_lock(&fts_input_dev->mutex);

	if (FTS_SYSFS_ECHO_ON(buf)) {
		FTS_INFO("[GESTURE]enable gesture");
		fts_gesture_data.enabled = true;
	} else if (FTS_SYSFS_ECHO_OFF(buf)) {
		FTS_INFO("[GESTURE]disable gesture");
		fts_gesture_data.enabled = false;
	}

	mutex_unlock(&fts_input_dev->mutex);

	return count;
}

/************************************************************************
 * Name: fts_gesture_buf_show
 *  Brief:
 *  Input: device, device attribute, char buf
 * Output:
 * Return:
 ***********************************************************************/
static ssize_t fts_gesture_buf_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int count;
	int i = 0;

	mutex_lock(&fts_input_dev->mutex);
	count = snprintf(buf, PAGE_SIZE, "Gesture ID: 0x%x\n",
			fts_gesture_data.header[0]);
	count += snprintf(buf + count, PAGE_SIZE, "Gesture PointNum: %d\n",
			fts_gesture_data.header[1]);
	count += snprintf(buf + count, PAGE_SIZE, "Gesture Point Buf:\n");

	for (i = 0; i < fts_gesture_data.header[1]; i++) {
		count += snprintf(buf + count, PAGE_SIZE, "%3d(%4d,%4d) ",
				i, fts_gesture_data.coordinate_x[i],
				fts_gesture_data.coordinate_y[i]);
		if ((i + 1)%4 == 0)
			count += snprintf(buf + count, PAGE_SIZE, "\n");
	}
	count += snprintf(buf + count, PAGE_SIZE, "\n");
	mutex_unlock(&fts_input_dev->mutex);

	return count;
}

/************************************************************************
 * Name: fts_gesture_buf_store
 *  Brief:
 *  Input: device, device attribute, char buf, char count
 * Output:
 * Return:
 ***********************************************************************/
static ssize_t fts_gesture_buf_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	/* place holder for future use */
	return -EPERM;
}

#ifdef CONFIG_MACH_MEIZU_M1721
static int fts_gesture_create_proc_symlinks(struct kernfs_node *sysfs_node_parent)
{
       int len, ret = 0;
       char *buf;
       char *double_tap_sysfs_node;
       struct proc_dir_entry *proc_entry_tp = NULL;
       struct proc_dir_entry *proc_symlink_tmp = NULL;

       buf = kzalloc(PATH_MAX, GFP_KERNEL);
       if (buf) {
               len = kernfs_path(sysfs_node_parent, buf, PATH_MAX);
               if (unlikely(len >= PATH_MAX)) {
                          pr_err("%s: Buffer too long: %d\n", __func__, len);
                          ret = -ERANGE;
                          goto exit;
               }
       }

       proc_entry_tp = proc_mkdir("touchpanel", NULL);
       if (proc_entry_tp == NULL) {
               pr_err("%s: Couldn't create touchpanel dir in procfs\n", __func__);
               ret = -ENOMEM;
               goto exit;
       }

       double_tap_sysfs_node = kzalloc(PATH_MAX, GFP_KERNEL);
       if (double_tap_sysfs_node)
               sprintf(double_tap_sysfs_node, "/sys%s/%s", buf, "enable_dt2w");
       proc_symlink_tmp = proc_symlink("enable_dt2w",
               proc_entry_tp, double_tap_sysfs_node);
       if (proc_symlink_tmp == NULL) {
               pr_err("%s: Couldn't create double_tap_enable symlink\n", __func__);
               ret = -ENOMEM;
               goto exit;
       }

exit:
       kfree(buf);
       kfree(double_tap_sysfs_node);
       return ret;
}
#endif

/*****************************************************************************
 *   Name: fts_create_gesture_sysfs
 *  Brief:
 *  Input:
 * Output:
 * Return: 0-success or others-error
 *****************************************************************************/
int fts_create_gesture_sysfs(struct i2c_client *client)
{
	int ret = 0;

	ret = sysfs_create_group(&client->dev.kobj, &fts_gesture_group);
	if (ret != 0) {
		FTS_ERROR("[GESTURE]fts_gesture_group(sysfs) create failed!");
		sysfs_remove_group(&client->dev.kobj, &fts_gesture_group);
		return ret;
	}

#ifdef CONFIG_MACH_MEIZU_M1721
	ret = fts_gesture_create_proc_symlinks(client->dev.kobj.sd);
	if (ret != 0) {
		FTS_ERROR("[GESTURE]fts_gesture_group(procfs) create failed!");
	}
#endif

	return 0;
}

/*****************************************************************************
 *   Name: fts_gesture_report
 *  Brief:
 *  Input:
 * Output:
 * Return:
 *****************************************************************************/
static void fts_gesture_report(struct input_dev *input_dev, int gesture_id)
{
	FTS_FUNC_ENTER();
	FTS_DEBUG("fts gesture_id==0x%x ", gesture_id);

	if (unlikely(gesture_id != 0x24)) {
		FTS_ERROR("fts unknown gesture: 0x%x ", gesture_id);
		return;
	}

	FTS_INFO("fts wakeup gesture");

	/* report event key */
	input_report_key(input_dev, KEY_WAKEUP, 1);
	input_sync(input_dev);
	input_report_key(input_dev, KEY_WAKEUP, 0);
	input_sync(input_dev);

	FTS_FUNC_EXIT();
}

/************************************************************************
 *   Name: fts_gesture_readdata
 *  Brief: read data from TP register
 *  Input:
 * Output:
 * Return: fail <0
 ***********************************************************************/
static int fts_gesture_read_buffer(struct i2c_client *client,
				u8 *buf, int read_bytes)
{
	int remain_bytes;
	int ret;
	int i;

	if (read_bytes <= I2C_BUFFER_LENGTH_MAXINUM) {
		ret = fts_i2c_read(client, buf, 1, buf, read_bytes);
	} else {
		ret = fts_i2c_read(client, buf, 1,
				buf, I2C_BUFFER_LENGTH_MAXINUM);
		remain_bytes = read_bytes - I2C_BUFFER_LENGTH_MAXINUM;
		for (i = 1; remain_bytes > 0; i++) {
			if (remain_bytes <= I2C_BUFFER_LENGTH_MAXINUM)
				ret = fts_i2c_read(client, buf, 0, buf
						+ I2C_BUFFER_LENGTH_MAXINUM * i,
						remain_bytes);
			else
				ret = fts_i2c_read(client, buf, 0, buf
						+ I2C_BUFFER_LENGTH_MAXINUM * i,
						I2C_BUFFER_LENGTH_MAXINUM);
			remain_bytes -= I2C_BUFFER_LENGTH_MAXINUM;
		}
	}

	return ret;
}

/************************************************************************
 *   Name: fts_gesture_fw
 *  Brief: Check IC's gesture recognise by FW or not
 *  Input:
 * Output:
 * Return: 1- FW  0- Driver
 ***********************************************************************/
static int fts_gesture_fw(void)
{
	int ret = 0;

	switch (chip_types.chip_idh) {
	case 0x54:
	case 0x58:
	case 0x64:
	case 0x87:
	case 0x86:
	case 0x80:
	case 0xE7:
		ret = 1;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}

/************************************************************************
 *   Name: fts_gesture_readdata
 *  Brief: read data from TP register
 *  Input:
 * Output:
 * Return: fail <0
 ***********************************************************************/
int fts_gesture_readdata(struct i2c_client *client)
{
	u8 buf[FTS_GESTRUE_POINTS * 4] = { 0 };
	int ret = -1;
	int i = 0;
	int gestrue_id = 0;
	int read_bytes = 0;
	u8 pointnum;

	FTS_FUNC_ENTER();
	/* init variable before read gesture point */
	memset(fts_gesture_data.header, 0, FTS_GESTRUE_POINTS_HEADER);
	memset(fts_gesture_data.coordinate_x, 0,
			FTS_GESTRUE_POINTS * sizeof(u16));
	memset(fts_gesture_data.coordinate_y, 0,
			FTS_GESTRUE_POINTS * sizeof(u16));

	buf[0] = FTS_REG_GESTURE_OUTPUT_ADDRESS;
	ret = fts_i2c_read(client, buf, 1, buf, FTS_GESTRUE_POINTS_HEADER);
	if (ret < 0) {
		FTS_ERROR("[GESTURE]Read gesture header data failed!!");
		FTS_FUNC_EXIT();
		return ret;
	}

	memcpy(fts_gesture_data.header, buf, FTS_GESTRUE_POINTS_HEADER);
	gestrue_id = buf[0];
	pointnum = buf[1];

	if (gestrue_id == GESTURE_SMALL_AREA) {
		FTS_INFO("[GESTURE] Wakeup gesture.");
		input_report_key(fts_input_dev, KEY_POWER, 1);
		input_sync(fts_input_dev);
		input_report_key(fts_input_dev, KEY_POWER, 0);
		input_sync(fts_input_dev);

	} else if (gestrue_id == GESTURE_LARGE_AREA) {
		FTS_INFO("[GESTURE] Large object detected.");
	} else if (fts_gesture_fw()) {
		/* FW recognize gesture */
		read_bytes = ((int)pointnum) * 4 + 2;
		buf[0] = FTS_REG_GESTURE_OUTPUT_ADDRESS;
		FTS_DEBUG("[GESTURE]PointNum=%d", pointnum);
		ret = fts_gesture_read_buffer(client, buf, read_bytes);
		if (ret < 0) {
			FTS_ERROR("[GESTURE]Read gesture touch data failed!!");
			FTS_FUNC_EXIT();
			return ret;
		}

		fts_gesture_report(fts_input_dev, gestrue_id);
		for (i = 0; i < pointnum; i++) {
			fts_gesture_data.coordinate_x[i] =
				(((s16) buf[0 + (4 * i + 2)]) & 0x0F) << 8
				| (((s16) buf[1 + (4 * i + 2)]) & 0xFF);
			fts_gesture_data.coordinate_y[i] =
				(((s16) buf[2 + (4 * i + 2)]) & 0x0F) << 8
				| (((s16) buf[3 + (4 * i + 2)]) & 0xFF);
		}


	} else {
		FTS_ERROR("[GESTURE]IC 0x%x need lib to support gestures.",
							chip_types.chip_idh);
	}

	FTS_FUNC_EXIT();

	return 0;
}

/*****************************************************************************
 *   Name: fts_gesture_recovery
 *  Brief: recovery gesture state when reset or power on
 *  Input:
 * Output:
 * Return:
 *****************************************************************************/
void fts_gesture_recovery(struct i2c_client *client)
{
	if (fts_gesture_data.enabled) {
		fts_i2c_write_reg(client, 0xD1, 0xff);
		fts_i2c_write_reg(client, 0xD2, 0xff);
		fts_i2c_write_reg(client, 0xD5, 0xff);
		fts_i2c_write_reg(client, 0xD6, 0xff);
		fts_i2c_write_reg(client, 0xD7, 0xff);
		fts_i2c_write_reg(client, 0xD8, 0xff);
		fts_i2c_write_reg(client, FTS_REG_GESTURE_EN, ENABLE);
	}
}

/*****************************************************************************
 *   Name: fts_gesture_suspend
 *  Brief:
 *  Input:
 * Output: None
 * Return: None
 *****************************************************************************/
int fts_gesture_suspend(struct i2c_client *i2c_client)
{
	int i;
	u8 state;

	FTS_FUNC_ENTER();

	/* gesture not enable, return immediately */
	if (!fts_gesture_data.enabled) {
		FTS_DEBUG("gesture is disabled");
		FTS_FUNC_EXIT();
		return -EINVAL;
	}

	for (i = 0; i < 5; i++) {
		fts_i2c_write_reg(i2c_client, 0xd1, 0xff);
		fts_i2c_write_reg(i2c_client, 0xd2, 0xff);
		fts_i2c_write_reg(i2c_client, 0xd5, 0xff);
		fts_i2c_write_reg(i2c_client, 0xd6, 0xff);
		fts_i2c_write_reg(i2c_client, 0xd7, 0xff);
		fts_i2c_write_reg(i2c_client, 0xd8, 0xff);
		fts_i2c_write_reg(i2c_client, FTS_REG_GESTURE_EN, 0x01);
		usleep_range(1000, 2000);
		fts_i2c_read_reg(i2c_client, FTS_REG_GESTURE_EN, &state);
		if (state == 1)
			break;
	}

	if (i >= 5) {
		FTS_ERROR("[GESTURE]Enter into gesture(suspend) failed!\n");
		FTS_FUNC_EXIT();
		return -EAGAIN;
	}

	// System should be able to handle interrupts from fts if gesture's enabled
	enable_irq_wake(i2c_client->irq);

	fts_gesture_data.enabled = true;
	FTS_DEBUG("[GESTURE]Enter into gesture(suspend) successfully!");
	FTS_FUNC_EXIT();
	return 0;
}

/*****************************************************************************
 *   Name: fts_gesture_resume
 *  Brief:
 *  Input:
 * Output: None
 * Return: None
 *****************************************************************************/
int fts_gesture_resume(struct i2c_client *client)
{
	int i;
	u8 state;

	FTS_FUNC_ENTER();

	/* gesture not enable, return immediately */
	if (fts_gesture_data.enabled == 0) {
		FTS_DEBUG("gesture is disabled");
		FTS_FUNC_EXIT();
		return -EINVAL;
	}

	for (i = 0; i < 5; i++) {
		fts_i2c_write_reg(client, FTS_REG_GESTURE_EN, 0x00);
		usleep_range(1000, 2000);
		fts_i2c_read_reg(client, FTS_REG_GESTURE_EN, &state);
		if (state == 0)
			break;
	}

	if (i >= 5) {
		FTS_ERROR("[GESTURE]Clear gesture(resume) failed!\n");
		return -EIO;
	}

	// System should be able to handle interrupts from fts if gesture's enabled
	disable_irq_wake(client->irq);

	FTS_FUNC_EXIT();

	return 0;
}

/*****************************************************************************
 *   Name: fts_gesture_init
 *  Brief:
 *  Input:
 * Output: None
 * Return: None
 *****************************************************************************/
int fts_gesture_init(struct input_dev *input_dev, struct i2c_client *client)
{
	FTS_FUNC_ENTER();
	input_set_capability(input_dev, EV_KEY, KEY_WAKEUP);
	__set_bit(KEY_WAKEUP, input_dev->keybit);

	fts_create_gesture_sysfs(client);
	fts_gesture_data.enabled = true;
	FTS_FUNC_EXIT();

	return 0;
}

/************************************************************************
 *   Name: fts_gesture_exit
 *  Brief: call when driver removed
 *  Input:
 * Output:
 * Return:
 ***********************************************************************/
int fts_gesture_exit(struct i2c_client *client)
{
	FTS_FUNC_ENTER();
	sysfs_remove_group(&client->dev.kobj, &fts_gesture_group);
	FTS_FUNC_EXIT();

	return 0;
}
#endif
