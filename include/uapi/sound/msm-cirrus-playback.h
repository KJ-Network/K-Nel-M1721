/* Copyright (c) 2016 Cirrus Logic, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef _UAPI_MSM_CIRRUS_SPK_PR_H
#define _UAPI_MSM_CIRRUS_SPK_PR_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define CIRRUS_GB_FFPORT	    0x00A1AF00
#define CIRRUS_GB_FBPORT		0x00A1BF00

#define CRUS_MODULE_ID_TX		0x00000002
#define CRUS_MODULE_ID_RX		0x00000001

/*
 * CRUS_PARAM_OPALUM
 *
 * For Opalum Speaker Protection
 * controls in cspl driver
 * 
 * Used in crus_afe_set_param in
 * Opalum functions of GB config    
 */
#define CRUS_PARAM_OPALUM        	0x00A1AF02

/*
 * CRUS_PARAM_RX/TX_SET_CALIB
 * Non-zero value to run speaker
 * calibration sequence
 *
 * uses crus_single_data_t apr pckt
 */
#define CRUS_PARAM_RX_SET_CALIB		0x00A1AF03
#define CRUS_PARAM_TX_SET_CALIB		0x00A1BF03

/*
 * CRUS_PARAM_RX/TX_SET_EXT_CONFIG
 * config string loaded from libfirmware
 * max of 7K parameters
 *
 * uses crus_external_config_t apr pckt
 */
#define CRUS_PARAM_RX_SET_EXT_CONFIG	0x00A1AF05
#define CRUS_PARAM_TX_SET_EXT_CONFIG	0x00A1BF08

/*
 * CRUS_PARAM_RX_GET_TEMP
 * get current Temp and calibration data
 *
 * CRUS_PARAM_TX_GET_TEMP_CAL
 * get results of calibration sequence
 *
 * uses cirrus_cal_result_t apr pckt
 */
#define CRUS_PARAM_RX_GET_TEMP		0x00A1AF07
#define CRUS_PARAM_TX_GET_TEMP_CAL	0x00A1BF06

/*
 * CRUS_PARAM_SET_TEMP_CAL
 * 
 * Set current Temp and calibration data
 */
#define CRUS_PARAM_SET_TEMP_CAL	    0x00A1AF08

/*
 * CRUS_PARAM_SET_F0_CAL
 * 
 * Set current F0 data
 */
#define CRUS_PARAM_SET_F0_CAL	    0x00A1BF05

#define CRUS_AFE_PARAM_ID_ENABLE	0x00010203

#define SPK_CAL_DATA_LENGTH 30
#define SPK_CAL_DATA_START_ADDR 0xD8

void msm_cirrus_callback(void);

#endif /* _UAPI_MSM_CIRRUS_SPK_PR_H */
 
