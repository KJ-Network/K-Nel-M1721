/* Copyright (c) 2015 Cirrus Logic, Inc.
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
#ifndef MSM_CIRRUS_PLAYBACK_H
#define MSM_CIRRUS_PLAYBACK_H

#include <linux/slab.h>
#include "../include/dsp/apr_audio-v2.h"
#include "../include/dsp/q6afe-v2.h"
#include "../include/dsp/q6audio-v2.h"
#include <sound/soc.h>
#include <uapi/sound/msm-cirrus-playback.h>

struct afe_custom_crus_set_config_t {
  struct apr_hdr hdr;
  struct afe_port_cmd_set_param_v2 param;
  struct afe_port_param_data_v2 data;
} __packed;

struct afe_custom_crus_get_config_t {
  struct apr_hdr hdr;
  struct afe_port_cmd_get_param_v2 param;
  struct afe_port_param_data_v2 data;
} __packed;

/* Payload struct for getting or setting one integer value from/to the DSP
 * module
 */
struct crus_single_data_t {
  int32_t value;
};

/* Payload struct for getting or setting two integer values from/to the DSP
 * module
 */
struct crus_dual_data_t {
  int32_t data1;
  int32_t data2;
};

/* Payload struct for getting or setting three integer values from/to the DSP
 * module
 */
struct crus_triple_data_t {
  int32_t data1;
  int32_t data2;
  int32_t data3;
};

#define APR_CHUNK_SIZE 4000

/* Payload struct for sending an external configuration string to the DSP
 * module
 */
struct crus_external_config_t {
  uint32_t total_size;
  uint32_t chunk_size;
  int32_t done;
  void *config;
};

struct crus_config_pkt_t {
  struct afe_custom_crus_set_config_t afe;
  struct crus_external_config_t ext;
};

struct crus_gb_cali_data {
	// Calibration data
	uint32_t temp_acc;
	uint32_t count;
	uint32_t ambient;

	// Return data
	int ret;
};

extern int afe_apr_send_pkt_crus(void *data, int index, int set);
int crus_afe_callback(void *payload, int size);
void msm_crus_pb_add_controls(struct snd_soc_platform *platform);

#endif /* _MSM_CIRRUS_PLAYBACK_H */
