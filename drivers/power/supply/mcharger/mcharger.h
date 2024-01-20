/*
 * Copyright (c) MeizuCustoms 2022
 * mCharge common header
 */

#include <linux/types.h>

/* mCharge Plus */
void charging_set_ta_current_pattern(bool increase);
int mcharger_p10_init(void);
int mcharger_p10_reset_ta_vchr(void);
int mcharger_p10_check_charger(void);
int mcharger_p10_start_algorithm(void);
void mcharger_p10_set_to_check_chr_type(bool check);
void mcharger_p10_set_is_enable(bool enable);
void mcharger_p10_set_is_cable_out_occur(bool out);
bool mcharger_p10_get_to_check_chr_type(void);
bool mcharger_p10_get_is_connect(void);
bool mcharger_p10_get_is_enable(void);

/* mCharge 2.0 */
int p20_plugout_reset(void);
int mcharger_p20_init(void);
int mcharger_p20_reset_ta_vchr(void);
int mcharger_p20_check_charger(void);
int mcharger_p20_start_algorithm(void);
void mcharger_p20_set_to_check_chr_type(bool check);
void mcharger_p20_set_is_enable(bool enable);
void mcharger_p20_set_is_cable_out_occur(bool out);
bool mcharger_p20_get_to_check_chr_type(void);
bool mcharger_p20_get_is_connect(void);
bool mcharger_p20_get_is_enable(void);

/* SMBCharger -> mCharge */
void mcharger_thread_wakeup(void);
int g_get_prop_batt_current_now(void);
int g_get_prop_batt_voltage_now(void);
int g_get_prop_batt_capacity(void);
bool charger_present(void);
int mcharger_set_high_usb_chg_current(int current_ma);
bool chr_type_is_dcp(void);
bool mcharger_is_plus_get(void);
void mcharger_is_plus_set(bool is_plus);
int smbchg_get_vchar_usbin(void);

/* Types */
typedef struct {
    int vbat;
    int vchar;
} p20_profile_t;
