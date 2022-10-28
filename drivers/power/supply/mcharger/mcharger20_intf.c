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

static struct mutex p20_access_lock;
static struct mutex p20_pmic_sync_lock;
static struct wakeup_source p20_suspend_lock;
static int p20_ta_vchr_org = 5000; /* mA */
static int p20_idx = -1;
static int p20_vbus = 5000; /* mA */
static bool p20_to_check_chr_type = true;
static bool
	p20_is_cable_out_occur; /* Plug out happened while detecting PE+20 */
static bool p20_is_connect;
static bool p20_is_enabled = true;
static int pe_flag1, pe_flag2, pe_count1, pe_count2;

int cptime[13][2];
struct timespec ptime[13];

static p20_profile_t p20_profile[] = {
	{3400, 9000}, {3500, 9000}, {3600, 9000},
	{3700, 9000}, {3800, 9000}, {3900, 9000},
	{4000, 9000}, {4100, 9000}, {4200, 9000},
	{4300, 9000},
};

static int p20_leave(void)
{
	int ret = 0;

	pr_debug("%s: starts\n", __func__);
	ret = mcharger_p20_reset_ta_vchr();
	if (ret < 0 || p20_is_connect) {
		pr_err(
			    "%s: failed, is_connect = %d, ret = %d\n", __func__,
			    p20_is_connect, ret);
		return ret;
	}

	pr_err("%s: OK\n", __func__);
	return ret;
}

static int p20_check_leave_status(void)
{
	int ret = 0;
	u32 ichg = 0, vchr = 0, ibat_now;
	bool current_sign = true;
    bool neg;

	pr_debug("%s: starts\n", __func__);

	/* PE+ leaves unexpectedly */
	vchr = smbchg_get_vchar_usbin();
	if (abs(vchr - p20_ta_vchr_org) < 1000) {
		pr_err(
			    "%s: PE+20 leave unexpectedly, recheck TA\n",
			    __func__);
		p20_to_check_chr_type = true;
		ret = p20_leave();
		if (ret < 0 || p20_is_connect)
			goto _err;

		return ret;
	}

	ichg = g_get_prop_batt_current_now();
    if (ichg < 0) {
        neg = 1;
        ichg /= -1000;
    } else {
        neg = 0;
        ichg /= 1000;
    }

    ibat_now = g_get_prop_batt_capacity();
    if (ibat_now > 85 && neg && ichg <= 999) {
        pe_flag1 = 1;
        pe_flag2 = 0;
        pe_count1 = 0;
        pe_count2 = 0;

        ret = p20_leave();
        if (ret < 0 || p20_is_connect)
			goto _err;
        
		pr_err(
            "%s: OK, SOC = (%d,%d), Ichg = %dmA, stop PE+20\n",
            __func__, ibat_now, 85, ichg);
    }

	return ret;

_err:
	pr_err("%s: failed, is_connect = %d, ret = %d\n",
		    __func__, p20_is_connect, ret);
	return ret;
}

static struct timespec tmp_ptime;
static int tmp_cptime;

static int tmp_dtime(int i)
{
    struct timespec ts;

    ts = timespec_sub(tmp_ptime, ptime[i-1]);
	pr_err("%s: tmp_ptime = %d, ptime[%d] = %d, %d\n", __func__,
		tmp_ptime.tv_nsec / 1000000, i - 1, ptime[i-1].tv_nsec / 1000000, ts.tv_nsec / 1000000);
    return ts.tv_nsec / 1000000;
}

static int dtime(int i)
{
	struct timespec time;

	time = timespec_sub(ptime[i], ptime[i-1]);
	pr_err("%s: ptime[%d] = %d, ptime[%d] = %d, %d\n", __func__,
		i, ptime[i].tv_nsec / 1000000, i - 1, ptime[i-1].tv_nsec / 1000000, time.tv_nsec / 1000000);
	return time.tv_nsec / 1000000;
}

#define PEOFFTIME 40
#define PEONTIME 90

// From MTK common Pump Express driver with Meizu stuff
static int charging_set_ta20_current_pattern(u32 chr_vol) {
    int value = (chr_vol - 5500) / 500;
    int flag, i, j = 0;
    
    mcharger_set_high_usb_chg_current(500);
	usleep_range(1000, 1200);
	mcharger_set_high_usb_chg_current(100);
    msleep(70);
    get_monotonic_boottime(&ptime[j++]);

	for (i = 4; i >= 0; i--) {
		flag = value & (1 << i);

        if (flag == 0) {
            mcharger_set_high_usb_chg_current(500);
			get_monotonic_boottime(&tmp_ptime);
            tmp_cptime = tmp_dtime(j);
            if (tmp_cptime <= 39)
                usleep_range(1000 * (40 - tmp_cptime), 1000 * (40 - tmp_cptime));
            get_monotonic_boottime(&ptime[j]);
            mcharger_set_high_usb_chg_current(100);

            cptime[j][0] = PEOFFTIME;
			cptime[j][1] = dtime(j);
			if (cptime[j][1] < 30 || cptime[j][1] > 65) {
				pr_info(
					"charging_set_ta20_current_pattern fail1: idx:%d target:%d actual:%d\n",
					i, PEOFFTIME, cptime[j][1]);
				return -EIO;
			}
			j++;

            mcharger_set_high_usb_chg_current(100);
			get_monotonic_boottime(&tmp_ptime);
            tmp_cptime = tmp_dtime(j);
            if (tmp_cptime <= 89)
                usleep_range(1000 * (90 - tmp_cptime), 1000 * (90 - tmp_cptime));
            get_monotonic_boottime(&ptime[j]);
            mcharger_set_high_usb_chg_current(500);

            cptime[j][0] = PEONTIME;
			cptime[j][1] = dtime(j);
			if (cptime[j][1] < 90 || cptime[j][1] > 115) {
				pr_info(
					"charging_set_ta20_current_pattern fail2: idx:%d target:%d actual:%d\n",
					i, PEOFFTIME, cptime[j][1]);
				return -EIO;
			}
			j++;
        } else {
            mcharger_set_high_usb_chg_current(100);
			get_monotonic_boottime(&tmp_ptime);
            tmp_cptime = tmp_dtime(j);
            if (tmp_cptime <= 89)
                usleep_range(1000 * (90 - tmp_cptime), 1000 * (90 - tmp_cptime));
            get_monotonic_boottime(&ptime[j]);
            mcharger_set_high_usb_chg_current(500);

            cptime[j][0] = PEONTIME;
			cptime[j][1] = dtime(j);
			if (cptime[j][1] < 90 || cptime[j][1] > 115) {
				pr_info(
					"charging_set_ta20_current_pattern fail3: idx:%d target:%d actual:%d\n",
					i, PEOFFTIME, cptime[j][1]);
				return -EIO;
			}
			j++;

            mcharger_set_high_usb_chg_current(500);
			get_monotonic_boottime(&tmp_ptime);
            tmp_cptime = tmp_dtime(j);
            if (tmp_cptime <= 39)
                usleep_range(1000 * (40 - tmp_cptime), 1000 * (40 - tmp_cptime));
            get_monotonic_boottime(&ptime[j]);
            mcharger_set_high_usb_chg_current(100);

            cptime[j][0] = PEOFFTIME;
			cptime[j][1] = dtime(j);
			if (cptime[j][1] < 30 || cptime[j][1] > 65) {
				pr_info(
					"charging_set_ta20_current_pattern fail4: idx:%d target:%d actual:%d\n",
					i, PEONTIME, cptime[j][1]);
				return -EIO;
			}
			j++;
        }
    }

    get_monotonic_boottime(&tmp_ptime);
    tmp_cptime = tmp_dtime(j);
    mcharger_set_high_usb_chg_current(500);
    if (tmp_cptime <= 159)
        usleep_range(1000 * (160 - tmp_cptime), 1000 * (160 - tmp_cptime));

	get_monotonic_boottime(&ptime[j]);
	cptime[j][0] = 160;
	cptime[j][1] = dtime(j);
    if (cptime[j][1] < 150 || cptime[j][1] > 240) {
		pr_info(
			"charging_set_ta20_current_pattern fail5: idx:%d target:%d actual:%d\n",
			i, PEOFFTIME, cptime[j][1]);
		return -EIO;
	}
	j++;

    mcharger_set_high_usb_chg_current(100);
    usleep_range(30000, 30000);
    mcharger_set_high_usb_chg_current(500);

    pr_info(
	"[charging_set_ta20_current_pattern]:chr_vol:%d bit:%d time:%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d!!\n",
	chr_vol, value,
	cptime[1][0], cptime[2][0], cptime[3][0], cptime[4][0], cptime[5][0],
	cptime[6][0], cptime[7][0], cptime[8][0], cptime[9][0], cptime[10][0], cptime[11][0]);

	pr_info(
	"[charging_set_ta20_current_pattern2]:chr_vol:%d bit:%d time:%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d!!\n",
	chr_vol, value,
	cptime[1][1], cptime[2][1], cptime[3][1], cptime[4][1], cptime[5][1],
	cptime[6][1], cptime[7][1], cptime[8][1], cptime[9][1], cptime[10][1], cptime[11][1]);

    mcharger_set_high_usb_chg_current(500);
    msleep(100);

    return 0;
}

static int __p20_set_ta_vchr(u32 chr_vol)
{
	int ret = 0;
    int a, delta_time, c, f, g;
    int i, j = 0;
	int flag, value;

	pr_debug("%s: starts\n", __func__);

    mcharger_is_plus_set(true);

	/* Not to set chr volt if cable is plugged out */
	if (!charger_present()) {
		pr_err("%s: failed, cable out\n", __func__);
		return -EIO;
	}

    ret = charging_set_ta20_current_pattern(chr_vol);
	if (ret) {
		pr_err("%s: ta20 current pattern set failed.\n", __func__);
		return ret;
	}

	pr_err("%s: OK voltage %d\n", __func__, smbchg_get_vchar_usbin());

    mcharger_is_plus_set(false);

	return ret;
}

static int p20_set_ta_vchr(u32 chr_volt)
{
	int ret = 0;
	int vchr_before, vchr_after, vchr_delta;
	const u32 sw_retry_cnt_max = 3;
	const u32 retry_cnt_max = 5;
	u32 sw_retry_cnt = 0, retry_cnt = 0;

	pr_debug("%s: starts\n", __func__);

	do {
		vchr_before = smbchg_get_vchar_usbin();
		ret = __p20_set_ta_vchr(chr_volt);
		vchr_after = smbchg_get_vchar_usbin();

		vchr_delta = abs(vchr_after - chr_volt);

		/* It is successful
		 * if difference to target is less than 500mA
		 */
		if (vchr_delta < 500 && ret == 0) {
			pr_err(
				"%s: OK, vchr = (%d, %d), vchr_target = %d\n",
				__func__, vchr_before, vchr_after, chr_volt);
			return ret;
		}

		if (ret == 0 || sw_retry_cnt >= sw_retry_cnt_max)
			retry_cnt++;
		else {
			msleep(2000);
			sw_retry_cnt++;
		}

		pr_err(
			"%s: retry_cnt = (%d, %d), vchr = (%d, %d), vchr_target = %d\n",
			__func__, sw_retry_cnt, retry_cnt, vchr_before,
			vchr_after, chr_volt);

	} while (!p20_is_cable_out_occur && charger_present() &&
		     retry_cnt < retry_cnt_max);
	ret = -EIO;
	pr_err(
		"%s: failed, vchr_org = %d, vchr_after = %d, target_vchr = %d\n",
		__func__, p20_ta_vchr_org, vchr_after, chr_volt);

	return ret;
}

static int p20_detect_ta(void)
{
	int ret;

	pr_debug("%s: starts\n", __func__);
	p20_ta_vchr_org = smbchg_get_vchar_usbin();

	if (abs(p20_ta_vchr_org - 8500) > 500)
		ret = p20_set_ta_vchr(8500);
	else
		ret = p20_set_ta_vchr(6500);

	if (ret < 0) {
		p20_to_check_chr_type = false;
		goto _err;
	}

	p20_is_connect = true;
	pr_err("%s: OK\n", __func__);

	return ret;
_err:
	p20_is_connect = false;
	pr_err("%s: failed, ret = %d\n", __func__, ret);
	return ret;
}

int p20_plugout_reset(void)
{
	int ret = 0;

	pr_debug("%s: starts\n", __func__);

	p20_to_check_chr_type = true;

	ret = mcharger_p20_reset_ta_vchr();
	if (ret < 0)
		goto _err;

	mcharger_p20_set_is_cable_out_occur(false);
	pr_err("%s: OK\n", __func__);

	return ret;
_err:
	pr_err("%s: failed, ret = %d\n", __func__, ret);

	return ret;
}

int mcharger_p20_init(void)
{
	wakeup_source_init(&p20_suspend_lock, "PE+20 TA charger suspend wakelock");
	mutex_init(&p20_access_lock);
	mutex_init(&p20_pmic_sync_lock);

	return 0;
}

int mcharger_p20_reset_ta_vchr(void)
{
	int ret = 0, chr_volt = 0;
	u32 retry_cnt = 0;

	pr_debug("%s: starts\n", __func__);

	/* Reset TA's charging voltage */
	do {
		mcharger_is_plus_set(1);
        mcharger_set_high_usb_chg_current(100);
		msleep(250);
        mcharger_set_high_usb_chg_current(500);
        mcharger_is_plus_set(0);
        msleep(250u);

		/* Check charger's voltage */
		chr_volt = smbchg_get_vchar_usbin();
		if (abs(chr_volt - p20_ta_vchr_org) <= 1000) {
			p20_vbus = chr_volt;
			p20_idx = -1;
			p20_is_connect = false;
			break;
		}

		retry_cnt++;
	} while (retry_cnt < 3);

	if (p20_is_connect) {
		pr_err("%s: failed, ret = %d\n", __func__,
			    ret);

		/*
		 * SET_TA20_RESET success but chr_volt does not reset to 5V
		 * set ret = -EIO to represent the case
		 */
		if (ret == 0)
			ret = -EIO;
		return ret;
	}

	pr_err("%s: OK\n", __func__);

	return ret;
}

int mcharger_p20_check_charger(void)
{
	int ret = 0;

	if (!p20_is_enabled) {
		pr_debug("%s: stop, PE+20 is disabled\n",
			    __func__);
		return ret;
	}

	mutex_lock(&p20_access_lock);
	__pm_stay_awake(&p20_suspend_lock);

	pr_debug("%s: starts\n", __func__);

	if (!charger_present() || p20_is_cable_out_occur)
		p20_plugout_reset();

	/*
	 * Not to check charger type or
	 * Not standard charger or
	 * SOC is not in range
	 */
	if (!(p20_to_check_chr_type && g_get_prop_batt_capacity() <= 84 
		&& (g_get_prop_batt_capacity() <= 0) < chr_type_is_dcp()))
		goto _out;

	ret = mcharger_p20_reset_ta_vchr();
	if (ret < 0)
		goto _out;

	ret = p20_detect_ta();
	if (ret < 0)
		goto _out;

	p20_to_check_chr_type = false;

	pr_err("%s: OK, to_check_chr_type = %d\n", __func__,
		    p20_to_check_chr_type);
	__pm_relax(&p20_suspend_lock);
	mutex_unlock(&p20_access_lock);

	return ret;
_out:
	pr_err(
		"%s: stop, SOC = (%d, %d, %d), to_check_chr_type = %d, ret = %d\n",
		__func__, g_get_prop_batt_capacity(), 0, 85, p20_to_check_chr_type, ret);

	__pm_relax(&p20_suspend_lock);
	mutex_unlock(&p20_access_lock);

	return ret;
}

int mcharger_p20_start_algorithm(void)
{
	int ret = 0;
	int i;
	int vbat, vbus, ichg;
	int pre_vbus, pre_idx;
	int tune = 0, pes = 0; /* For log, to know the state of PE+20 */
	bool current_sign = true;
	u32 size;

	if (!p20_is_enabled) {
		pr_err("%s: stop, PE+20 is disabled\n",
			    __func__);
		return ret;
	}

	mutex_lock(&p20_access_lock);
	__pm_stay_awake(&p20_suspend_lock);
	pr_debug("%s: starts\n", __func__);

	if (!charger_present() || p20_is_cable_out_occur)
		p20_plugout_reset();

	if (!p20_is_connect) {
		ret = -EIO;
		pr_err("%s: stop, PE+20 is not connected\n",
			    __func__);
		__pm_relax(&p20_suspend_lock);
		mutex_unlock(&p20_access_lock);
		return ret;
	}

	vbat = smbchg_get_vchar_usbin();
	vbus = g_get_prop_batt_voltage_now();
	ichg = g_get_prop_batt_current_now();

	pre_vbus = p20_vbus;
	pre_idx = p20_idx;

	ret = p20_check_leave_status();
	if (!p20_is_connect || ret < 0) {
		pes = 1;
		tune = 0;
        goto _out;
	}

    size = ARRAY_SIZE(p20_profile);
	for (i = 0; i < size; i++) {
		tune = 0;

		/* Exceed this level, check next level */
		if (vbat > (p20_profile[i].vbat + 100))
			continue;

		/* If vbat is still 30mV larger than the lower level
		 * Do not down grade
		 */
		if (i < p20_idx && vbat > (p20_profile[i].vbat + 30))
			continue;

		if (p20_vbus != p20_profile[i].vchar)
			tune = 1;

		p20_vbus = p20_profile[i].vchar;
		p20_idx = i;

		if (abs(vbus - p20_vbus) >= 1000)
			tune = 2;
        else {
            if (p20_vbus == p20_profile[i].vchar) {
                pe_flag1 = 1;
                pe_flag2 = 0;
                pe_count1 = 0;
                pe_count2 = 0;
                pes = 2;
                tune = 0;
                goto _out;
            }

            if (pe_flag1 == 1) {
                if (pe_count1 > 9) {
                    pe_count1 = 0;
                    pe_flag1 = 0;
                    pe_flag2 = 1;
                } else {
                    ++pe_count1;
                }
            }

            if (pe_flag2 == 1)
            {
                if (pe_count2 > 9)
                    pe_count2 = 0;
                else
                    ++pe_count2;
            }

            pes = 2;

            if ((pe_flag1 == 1 || pe_flag2 == 1) && !pe_count2) {
                pes = 2;
                ret = p20_set_ta_vchr(p20_vbus);
                if (ret < 0)
                    p20_leave();
            }
        }

		if (tune != 0) {
			ret = p20_set_ta_vchr(p20_vbus);
			if (ret < 0)
				p20_leave();
		}
		break;
	}
	pes = 2;

_out:
	pr_err(
		    "%s: vbus = (%d, %d), idx = (%d, %d), I = (%d, %d)\n",
		    __func__, pre_vbus, p20_vbus, pre_idx, p20_idx,
		    (int)current_sign, (int)ichg / 10);

	pr_err(
		"%s: SOC = %d, is_connect = %d, tune = %d, pes = %d, vbat = %d, ret = %d\n",
		__func__, g_get_prop_batt_capacity(), p20_is_connect, tune, pes, vbat, ret);
	__pm_relax(&p20_suspend_lock);
	mutex_unlock(&p20_access_lock);

	return ret;
}

void mcharger_p20_set_to_check_chr_type(bool check)
{
	mutex_lock(&p20_access_lock);
	__pm_stay_awake(&p20_suspend_lock);

	pr_err("%s: check = %d\n", __func__, check);
	p20_to_check_chr_type = check;

	__pm_relax(&p20_suspend_lock);
	mutex_unlock(&p20_access_lock);
}

void mcharger_p20_set_is_enable(bool enable)
{
	mutex_lock(&p20_access_lock);
	__pm_stay_awake(&p20_suspend_lock);

	pr_err("%s: enable = %d\n", __func__, enable);
	p20_is_enabled = enable;

	__pm_relax(&p20_suspend_lock);
	mutex_unlock(&p20_access_lock);
}

void mcharger_p20_set_is_cable_out_occur(bool out)
{
	pr_err("%s: out = %d\n", __func__, out);
	mutex_lock(&p20_pmic_sync_lock);
	p20_is_cable_out_occur = out;
	mutex_unlock(&p20_pmic_sync_lock);
}

bool mcharger_p20_get_to_check_chr_type(void)
{
	return p20_to_check_chr_type;
}

bool mcharger_p20_get_is_connect(void)
{
	/*
	 * Cable out is occurred,
	 * but not execute plugout_reset yet
	 */
	if (p20_is_cable_out_occur)
		return false;

	return p20_is_connect;
}

bool mcharger_p20_get_is_enable(void)
{
	return p20_is_enabled;
}
