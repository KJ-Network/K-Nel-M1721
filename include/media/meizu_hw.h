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

#include <linux/types.h>

#ifndef __MEIZU_HW_H
#define __MEIZU_HW_H

// Workaround to use MeizuHW with other drivers
#ifdef MZHW_USED_FOR_CAMERA
#include "msm_sensor.h"
#include "msm_cci.h"

/* camera_v2 changes */
struct meizu_camera_sensor {
    unsigned int id;
    char name[24];
    char lens[32];
};

struct meizu_camera_data {
    struct meizu_camera_sensor back;
    struct meizu_camera_sensor front;
    struct meizu_camera_sensor bokeh;
};

int meizu_sensor_parse_id(struct meizu_camera_data *camera,
                          struct msm_sensor_ctrl_t *s_ctrl);
int meizu_camera_print_data(struct meizu_camera_data *camera);
#endif

// Cool logs
#define mz_info(fmt, args...) pr_info("mzhw: %s: " fmt, __func__, ##args)
#define mz_warn(fmt, args...) pr_warn("mzhw: %s: " fmt, __func__, ##args)
#define mz_err(fmt, args...) pr_err("mzhw: %s: " fmt, __func__, ##args)

/* MeizuHW class */
int meizu_hw_class_init(void);

/* DWC3-MSM changes */
bool is_usb_charge_only_mode(void); 

#endif /* __MEIZU_HW_H */
