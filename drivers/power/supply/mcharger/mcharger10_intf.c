/*
 * Copyright (C) 2016 MediaTek Inc.
 * Copyright (C) 2022 MeizuCustoms
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#include "mcharger.h"
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/ktime.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/kobject.h>
#include <linux/pm.h>
#include <linux/printk.h>
#include <linux/power_supply.h>
#include <linux/pm_wakeup.h>
#include <linux/slab.h>

static struct mutex p10_access_lock;
static struct mutex p10_pmic_sync_lock;
static struct wakeup_source p10_suspend_lock;
static int p10_ta_vchr_org = 5000; /* mA */
static bool p10_to_check_chr_type = true;
static bool p10_to_tune_ta_vchr = true;
static bool p10_is_cable_out_occur; /* Plug out happened while detect PE+ */
static bool p10_is_connect;
static bool p10_is_enabled = true;
static int d_time;
static bool ta_9v_support;

void msleep_time(int dtime, int time)
{
  if (time > dtime)
    usleep_range(1000 * (time - dtime), 1000 * (time - dtime));
}

static int mcharger_set_high_usb_chg_current_time(int current_ma)
{
	struct timespec time1;
    struct timespec time2;
    struct timespec time;

    get_monotonic_boottime(&time1);
    mcharger_set_high_usb_chg_current(current_ma);
    get_monotonic_boottime(&time2);

	time = timespec_sub(time2, time1);
	return time.tv_nsec / 1000000;
}

// From MTK common Pump Express driver with Meizu stuff
void charging_set_ta_current_pattern(bool increase)
{
  if (charger_present()) {
    if (increase) {
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep(85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 281);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 281);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 281);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 485);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 50);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 200);
    } else {
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep(85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 281);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 281);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 281);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 85);
      d_time = mcharger_set_high_usb_chg_current_time(500);
      msleep_time(d_time, 485);
      d_time = mcharger_set_high_usb_chg_current_time(100);
      msleep_time(d_time, 50);
      d_time = mcharger_set_high_usb_chg_current_time(500);
    }
  }
}

static int p10_leave(bool disable_charging)
{
	int ret = 0;

	pr_debug("%s: starts\n", __func__);

	/* Decrease TA voltage to 5V */
	ret = mcharger_p10_reset_ta_vchr();
	if (ret < 0 || p10_is_connect)
		goto _err;

	pr_err( "%s: OK\n", __func__);
	return ret;

_err:
	pr_err( "%s: failed, is_connect = %d, ret = %d\n",
		    __func__, p10_is_connect, ret);
	return ret;
}

static int p10_check_leave_status(void)
{
	int ret = 0;
	u32 ichg = 0, vchr = 0;
	bool current_sign;

	pr_debug("%s: starts\n", __func__);

	/* PE+ leaves unexpectedly */
	vchr = smbchg_get_vchar_usbin();
	if (abs(vchr - p10_ta_vchr_org) < 1000) {
		pr_err(
			    "%s: PE+ leave unexpectedly, recheck TA\n",
			    __func__);
		p10_to_check_chr_type = true;
		ret = p10_leave(true);
		if (ret < 0 || p10_is_connect)
			goto _err;

		return ret;
	}

    ichg = g_get_prop_batt_current_now();
    if (ichg < 0) {
        current_sign = 1;
        ichg /= -1000;
    } else {
        current_sign = 0;
        ichg /= 1000;
    }

	/* Check SOC & Ichg */
	if (g_get_prop_batt_capacity() > 85 && current_sign && ichg < 1000) {
		ret = p10_leave(true);
		if (ret < 0 || p10_is_connect)
			goto _err;
		pr_err(
			    "%s: OK, SOC = (%d,%d), Ichg = %dmA, stop PE+\n",
			    __func__, g_get_prop_batt_capacity(), 85, ichg);
	}

	return ret;

_err:
	pr_err( "%s: failed, is_connect = %d, ret = %d\n",
		    __func__, p10_is_connect, ret);
	return ret;
}

static int __p10_increase_ta_vchr(void)
{
	int ret = 0;

    mcharger_is_plus_set(1);
    charging_set_ta_current_pattern(1);
    mcharger_is_plus_set(0);

	return ret;
}

static int p10_increase_ta_vchr(u32 vchr_target)
{
	int ret = 0;
	int vchr_before, vchr_after;
	u32 retry_cnt = 0;

	do {
		vchr_before = smbchg_get_vchar_usbin();
		__p10_increase_ta_vchr();
		vchr_after = smbchg_get_vchar_usbin();

		if (abs(vchr_after - vchr_target) <= 1000) {
			pr_err( "%s: OK\n", __func__);
			return ret;
		}
		pr_err(
			"%s: retry, cnt = %d, vchr = (%d, %d), vchr_target = %d\n",
			__func__, retry_cnt, vchr_before, vchr_after,
			vchr_target);

		retry_cnt++;
	} while (!charger_present() && retry_cnt < 3);

	ret = -EIO;
	pr_err(
		    "%s: failed, vchr = (%d, %d), vchr_target = %d\n", __func__,
		    vchr_before, vchr_after, vchr_target);

	return ret;
}

static int p10_detect_ta(void)
{
	int ret = 0;

	pr_debug("%s: starts\n", __func__);

	p10_ta_vchr_org = smbchg_get_vchar_usbin();
	ret = p10_increase_ta_vchr(7000); /* mA */

	if (ret == 0) {
		p10_is_connect = true;
		pr_err( "%s: OK, is_connect = %d\n", __func__,
			    p10_is_connect);
		return ret;
	}

	/* Detect PE+ TA failed */
	p10_is_connect = false;
	p10_to_check_chr_type = false;

	pr_err( "%s: failed, is_connect = %d\n", __func__,
		    p10_is_connect);

	return ret;
}

static int p10_init_ta(void)
{
	int ret = 0;

	pr_debug("%s: starts\n", __func__);
	if (ta_9v_support)
		p10_to_tune_ta_vchr = true;

	return ret;
}

static int p10_plugout_reset(void)
{
	int ret = 0;

	pr_debug("%s: starts\n", __func__);

	p10_to_check_chr_type = true;

	ret = mcharger_p10_reset_ta_vchr();
	if (ret < 0)
		goto _err;

	/* Set cable out occur to false */
	mcharger_p10_set_is_cable_out_occur(false);
	pr_err( "%s: OK\n", __func__);
	return ret;

_err:
	pr_err( "%s: failed, ret = %d\n", __func__, ret);

	return ret;
}

int mcharger_p10_init(void)
{
	wakeup_source_init(&p10_suspend_lock, "PE+ TA charger suspend wakelock");
	mutex_init(&p10_access_lock);
	mutex_init(&p10_pmic_sync_lock);
    ta_9v_support = 1;
	return 0;
}

int mcharger_p10_reset_ta_vchr(void)
{
	int ret = 0, chr_volt = 0;
	u32 retry_cnt = 0;

	pr_debug("%s: starts\n", __func__);

	do {
		mcharger_is_plus_set(1);
        mcharger_set_high_usb_chg_current(100);
        msleep(500);
        mcharger_is_plus_set(0);

		/* Check charger's voltage */
		chr_volt = smbchg_get_vchar_usbin();
		if (abs(chr_volt - p10_ta_vchr_org) <= 1000) {
			p10_is_connect = false;
			break;
		}

		retry_cnt++;
	} while (retry_cnt < 3);

	if (p10_is_connect) {
		pr_err( "%s: failed, ret = %d\n", __func__,
			    ret);
		/*
		 * SET_INPUT_CURRENT success but chr_volt does not reset to 5V
		 * set ret = -EIO to represent the case
		 */
		ret = -EIO;
		return ret;
	}

	pr_err( "%s: OK\n", __func__);

	return ret;
}

int mcharger_p10_check_charger(void)
{
	int ret = 0;

	if (mcharger_p20_get_is_connect()) {
		pr_debug( "%s: stop, PE+20 is connected\n",
			    __func__);
		return ret;
	}

	if (!p10_is_enabled) {
		pr_debug( "%s: stop, PE+ is disabled\n",
			    __func__);
		return ret;
	}

	/* Lock */
	mutex_lock(&p10_access_lock);
	__pm_stay_awake(&p10_suspend_lock);

	pr_debug("%s: starts\n", __func__);

	if (!charger_present() || p10_is_cable_out_occur)
		p10_plugout_reset();

	/* Not to check charger type or
	 * Not standard charger or
	 * SOC is not in range
	 */
	if (!(p10_to_check_chr_type && g_get_prop_batt_capacity() <= 84 
		&& (g_get_prop_batt_capacity() <= 0) < chr_type_is_dcp()))
		goto _err;

	/* Reset/Init/Detect TA */
	ret = mcharger_p10_reset_ta_vchr();
	if (ret < 0)
		goto _err;

	ret = p10_init_ta();
	if (ret < 0)
		goto _err;

	ret = p10_detect_ta();
	if (ret < 0)
		goto _err;

	p10_to_check_chr_type = false;

	/* Unlock */
	__pm_relax(&p10_suspend_lock);
	mutex_unlock(&p10_access_lock);
	pr_err( "%s: OK, to_check_chr_type = %d\n", __func__,
		    p10_to_check_chr_type);
	return ret;

_err:
	/* Unlock */
	__pm_relax(&p10_suspend_lock);
	mutex_unlock(&p10_access_lock);
	pr_err(
		"%s: stop, SOC = %d, to_check_chr_type = %d, ret = %d\n",
		__func__, g_get_prop_batt_capacity(), p10_to_check_chr_type, ret);
	return ret;
}

int mcharger_p10_start_algorithm(void)
{
	int ret = 0, chr_volt;

	if (mcharger_p20_get_is_connect()) {
		pr_debug( "%s: stop, PE+20 is connected\n",
			    __func__);
		return ret;
	}

	if (!p10_is_enabled) {
		pr_debug( "%s: stop, PE+ is disabled\n",
			    __func__);
		return ret;
	}

	/* Lock */
	mutex_lock(&p10_access_lock);
	__pm_stay_awake(&p10_suspend_lock);

	pr_debug("%s: starts\n", __func__);

	if (!charger_present() || p10_is_cable_out_occur)
		p10_plugout_reset();

	/* TA is not connected */
	if (!p10_is_connect) {
		ret = -EIO;
		pr_debug( "%s: stop, PE+ is not connected\n",
			    __func__);
		goto _out;
	}

	/* No need to tune TA */
	if (!p10_to_tune_ta_vchr) {
		ret = p10_check_leave_status();
		pr_debug( "%s: stop, not to tune TA vchr\n",
			    __func__);
		goto _out;
	}

	p10_to_tune_ta_vchr = false;

	/* Increase TA voltage to 9V */
	if (ta_9v_support) {
		ret = p10_increase_ta_vchr(9000); /* mA */
		if (ret < 0) {
			pr_err(
				    "%s: failed, cannot increase to 9V\n",
				    __func__);
			goto _err;
		}

		/* Successfully, increase to 9V */
		pr_err( "%s: output 9V ok\n", __func__);
	}

	chr_volt = smbchg_get_vchar_usbin();

	pr_err(
		    "%s: vchr_org = %d, vchr_after = %d, delta = %d\n",
		    __func__, p10_ta_vchr_org, chr_volt,
		    chr_volt - p10_ta_vchr_org);
	pr_err( "%s: OK\n", __func__);

	__pm_relax(&p10_suspend_lock);
	mutex_unlock(&p10_access_lock);
	return ret;

_err:
	p10_leave(false);
_out:
	chr_volt = smbchg_get_vchar_usbin();
	pr_err(
		    "%s: vchr_org = %d, vchr_after = %d, delta = %d\n",
		    __func__, p10_ta_vchr_org, chr_volt,
		    chr_volt - p10_ta_vchr_org);

	__pm_relax(&p10_suspend_lock);
	mutex_unlock(&p10_access_lock);

	return ret;
}

/* PE+ set functions */
void mcharger_p10_set_to_check_chr_type(bool check)
{
	mutex_lock(&p10_access_lock);
	__pm_stay_awake(&p10_suspend_lock);

	pr_err( "%s: check = %d\n", __func__, check);
	p10_to_check_chr_type = check;

	__pm_relax(&p10_suspend_lock);
	mutex_unlock(&p10_access_lock);
}

void mcharger_p10_set_is_enable(bool enable)
{
	mutex_lock(&p10_access_lock);
	__pm_stay_awake(&p10_suspend_lock);

	pr_err( "%s: enable = %d\n", __func__, enable);
	p10_is_enabled = enable;

	__pm_relax(&p10_suspend_lock);
	mutex_unlock(&p10_access_lock);
}

void mcharger_p10_set_is_cable_out_occur(bool out)
{
	pr_err( "%s: out = %d\n", __func__, out);
	mutex_lock(&p10_pmic_sync_lock);
	p10_is_cable_out_occur = out;
	mutex_unlock(&p10_pmic_sync_lock);
}

/* PE+ get functions */
bool mcharger_p10_get_to_check_chr_type(void)
{
	return p10_to_check_chr_type;
}

bool mcharger_p10_get_is_connect(void)
{
	/*
	 * Cable out is occurred,
	 * but not execute plugout_reset yet
	 */
	if (p10_is_cable_out_occur)
		return false;

	return p10_is_connect;
}

bool mcharger_p10_get_is_enable(void)
{
	return p10_is_enabled;
}
