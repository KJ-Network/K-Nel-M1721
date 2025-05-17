/* Copyright (c) 2015, The Linux Foundation. All rights reserved.
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/acpi.h>
#include <linux/compat.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/firmware.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>

#include "msm-cirrus-playback.h"

extern int afe_apr_send_pkt_crus(void *data, int index, int set);

static int msm_cirrus_get_temp_cal(void);
static int msm_cirrus_write_calibration_data(struct crus_gb_cali_data *cali);
static int msm_cirrus_config_opalum_music(void);
static int msm_cirrus_config_opalum_voice(void);
static int msm_cirrus_flash_rx_config(void);
static int msm_cirrus_flash_rx_new_config(void);
static int msm_cirrus_flash_tx_config(void);
static struct crus_gb_cali_data msm_cirrus_get_speaker_calibration_data(void);

static struct device *crus_gb_device;
static atomic_t crus_gb_misc_usage_count;

static struct crus_single_data_t crus_enable;

static int32_t *crus_gb_get_buffer;
static atomic_t crus_gb_get_param_flag;
struct mutex crus_gb_get_param_lock;
struct mutex crus_gb_lock;
static int crus_gb_enable;
static int crus_gb_cfg, crus_gb_ext_cfg;
static int music_config_loaded = false;
static int voice_config_loaded = false;
static int cirrus_fb_port = AFE_PORT_ID_QUATERNARY_MI2S_TX;
static int cirrus_ff_port = AFE_PORT_ID_QUATERNARY_MI2S_RX;
static struct crus_gb_cali_data g_cali;

void *crus_gen_afe_set_header(int length, int port, int module, int param) {
  struct afe_custom_crus_set_config_t *config = NULL;
  int size = sizeof(struct afe_custom_crus_set_config_t) + length;
  int index = afe_get_port_index(port);
  uint16_t payload_size = sizeof(struct afe_port_param_data_v2) + length;

  /* Allocate memory for the message */
  config = kzalloc(size, GFP_KERNEL);
  if (!config)
    return NULL;

  /* Set header section */
  config->hdr.hdr_field = 592;
  config->hdr.pkt_size = size;
  config->hdr.src_svc = 4;
  config->hdr.src_domain = 5;
  config->hdr.src_port = 0;
  config->hdr.dest_svc = 4;
  config->hdr.dest_domain = 4;
  config->hdr.dest_port = 0;
  config->hdr.token = index;
  config->hdr.opcode = 65775;

  /* Set param section */
  config->param.port_id = (uint16_t)port;
  config->param.payload_address_lsw = 0;
  config->param.payload_address_msw = 0;
  config->param.mem_map_handle = 0;
  /* max data size of the param_ID/module_ID combination */
  config->param.payload_size = payload_size;

  /* Set data section */
  config->data.module_id = (uint32_t)module;
  config->data.param_id = (uint32_t)param;
  config->data.reserved = 0; /* Must be set to 0 */
  /* actual size of the data for the module_ID/param_ID pair */
  config->data.param_size = length;

  return (void *)config;
}

int crus_afe_set_param(int port, int module, int param, int data_size,
                       void *data_ptr) {
  struct afe_custom_crus_set_config_t *config = NULL;
  int index = afe_get_port_index(port);
  int ret = 0;

  pr_debug("%s: port = %d module = %d param = 0x%x data_size = %d\n", __func__,
          port, module, param, data_size);

  mutex_lock(&crus_gb_lock);

  config = crus_gen_afe_set_header(data_size, port, module, param);
  if (config == NULL) {
    pr_err("%s: Memory allocation failed!\n", __func__);
    return -ENOMEM;
  }

  memcpy((u8 *)config + sizeof(struct afe_custom_crus_set_config_t),
         (u8 *)data_ptr, data_size);

  pr_debug("%s: Preparing to send apr packet.\n", __func__);

  ret = afe_apr_send_pkt_crus(config, index, 1);
  if (ret) {
    pr_err("%s: crus set_param for port %d failed with code %d\n", __func__,
           port, ret);
  } else {
    pr_debug("%s: crus set_param sent packet with param id 0x%08x to module "
            "0x%08x.\n",
            __func__, param, module);
  }

  mutex_unlock(&crus_gb_lock);

  kfree(config);
  return ret;
}

void *crus_gen_afe_get_header(int length, int port, int module, int param) {
  struct afe_custom_crus_get_config_t *config = NULL;
  int size = sizeof(struct afe_custom_crus_get_config_t);
  int index = afe_get_port_index(port);
  uint16_t payload_size = sizeof(struct afe_port_param_data_v2) + length;

  /* Allocate memory for the message */
  config = kzalloc(size, GFP_KERNEL);
  if (!config)
    return NULL;

  /* Set header section */
  config->hdr.hdr_field = 592;
  config->hdr.pkt_size = size;
  config->hdr.src_svc = 4;
  config->hdr.src_domain = 5;
  config->hdr.src_port = 0;
  config->hdr.dest_svc = 4;
  config->hdr.dest_domain = 4;
  config->hdr.dest_port = 0;
  config->hdr.token = index;
  config->hdr.opcode = 65776;

  /* Set param section */
  config->param.port_id = (uint16_t)port;
  config->param.payload_address_lsw = 0;
  config->param.payload_address_msw = 0;
  config->param.mem_map_handle = 0;
  config->param.module_id = (uint32_t)module;
  config->param.param_id = (uint32_t)param;
  /* max data size of the param_ID/module_ID combination */
  config->param.payload_size = payload_size;

  /* Set data section */
  config->data.module_id = (uint32_t)module;
  config->data.param_id = (uint32_t)param;
  config->data.reserved = 0; /* Must be set to 0 */
  /* actual size of the data for the module_ID/param_ID pair */
  config->data.param_size = length;

  return (void *)config;
}

int crus_afe_get_param(int port, int module, int param, int length,
                       void *data) {
  struct afe_custom_crus_get_config_t *config = NULL;
  int index = afe_get_port_index(port);
  int ret = 0;

  pr_debug("%s: port = %d module = %d param = 0x%x length = %d\n", __func__,
          port, module, param, length);

  config = (struct afe_custom_crus_get_config_t *)crus_gen_afe_get_header(
      length, port, module, param);
  if (config == NULL) {
    pr_err("%s: Memory allocation failed!\n", __func__);
    return -ENOMEM;
  }

  pr_debug("%s: Preparing to send apr packet\n", __func__);

  mutex_lock(&crus_gb_get_param_lock);
  atomic_set(&crus_gb_get_param_flag, 0);

  crus_gb_get_buffer = kzalloc(config->param.payload_size + 16, GFP_KERNEL);

  ret = afe_apr_send_pkt_crus(config, index, 0);
  if (ret)
    pr_err("%s: crus get_param for port %d failed with code %d\n", __func__,
           port, ret);
  else
    pr_debug("%s: crus get_param sent packet with param id 0x%08x to module "
            "0x%08x.\n",
            __func__, param, module);

  /* Wait for afe callback to populate data */
  while (!atomic_read(&crus_gb_get_param_flag))
    msleep(1);

  pr_debug("CRUS CRUS_AFE_GET_PARAM: returned data = [4]: %d, [5]: %d\n",
          crus_gb_get_buffer[4], crus_gb_get_buffer[5]);

  /* Copy from dynamic buffer to return buffer */
  memcpy((u8 *)data, &crus_gb_get_buffer[4], length);

  kfree(crus_gb_get_buffer);

  mutex_unlock(&crus_gb_get_param_lock);

  kfree(config);
  return ret;
}

int crus_afe_send_config(const char *data, int32_t module) {
  struct crus_config_pkt_t *pkt = NULL;
  int length = strlen(data) + 1;
  int ret = 0;
  int index, port;
  uint32_t param = 0;
  int mem_size = 0;
  int sent = 0;
  int chars_to_send = 0;

  pr_debug("%s: called with module_id = %x, string length = %d\n", __func__,
          module, length);

  /* Destination settings for message */
  if (module == CIRRUS_GB_FFPORT) {
    param = CRUS_PARAM_RX_SET_EXT_CONFIG;
    index = afe_get_port_index(cirrus_ff_port);
    port = cirrus_ff_port;
  } else if (module == CIRRUS_GB_FBPORT) {
    param = CRUS_PARAM_TX_SET_EXT_CONFIG;
    index = afe_get_port_index(cirrus_fb_port);
    port = cirrus_fb_port;
  } else {
    pr_err("%s: Received invalid port parameter %d\n", __func__, module);
    return -EINVAL;
  }

  if (length > APR_CHUNK_SIZE)
    mem_size = APR_CHUNK_SIZE;
  else
    mem_size = length;

  pkt = kmalloc(mem_size + sizeof(struct crus_config_pkt_t), GFP_KERNEL);
  if (!pkt) {
    pr_err("%s: failed to allocate memory\n", __func__);
    return 1;
  }

  pkt->ext.total_size = length;
  pkt->afe.hdr.hdr_field = 592;
  pkt->afe.hdr.src_svc = 4;
  pkt->afe.hdr.dest_svc = 4;
  pkt->afe.hdr.dest_domain = 4;
  pkt->afe.hdr.token = index;
  pkt->afe.hdr.opcode = 65775;
  pkt->afe.param.payload_size = mem_size + sizeof(struct crus_external_config_t) +
                                sizeof(struct afe_port_param_data_v2);
  pkt->afe.data.param_size = mem_size + sizeof(struct crus_external_config_t);
  pkt->afe.hdr.pkt_size = mem_size + sizeof(struct crus_config_pkt_t);
  pkt->afe.hdr.src_domain = 5;
  pkt->afe.hdr.src_port = 0;
  pkt->afe.hdr.dest_port = 0;
  pkt->afe.param.port_id = port;
  pkt->afe.param.payload_address_lsw = 0;
  pkt->afe.param.payload_address_msw = 0;
  pkt->afe.param.mem_map_handle = 0;
  pkt->afe.data.module_id = module;
  pkt->afe.data.param_id = param;
  pkt->afe.data.reserved = 0;

  while (sent < length) {
    chars_to_send = length - sent;
    if (chars_to_send > APR_CHUNK_SIZE) {
      chars_to_send = APR_CHUNK_SIZE;
      pkt->ext.done = 0;
    } else {
      pkt->ext.done = 1;
    }

    /* Configure per message parameter settings */
    memcpy(&pkt->ext.config, &data[sent], chars_to_send);
    pkt->ext.chunk_size = chars_to_send;

    /* Send the actual message */
    pr_debug("%s: Preparing to send apr packet.\n", __func__);
    ret = afe_apr_send_pkt_crus(pkt, index, 1);
    if (ret)
      pr_err("%s: crus set_param for port %d failed with code %d\n", __func__,
             port, ret);
    else
      pr_debug("%s: crus set_param sent packet with param id 0x%08x to module "
             "0x%08x.\n",
             __func__, param, module);

    sent += chars_to_send;
  }

  kfree(pkt);
  return ret;
}

int crus_afe_callback(void *payload, int size) {
  uint32_t *payload32 = payload;

  pr_debug("Cirrus AFE CALLBACK: size = %d\n", size);

  switch (payload32[1]) {
  case CIRRUS_GB_FFPORT:
  case CIRRUS_GB_FBPORT:
    memcpy(crus_gb_get_buffer, payload32, size);
    atomic_set(&crus_gb_get_param_flag, 1);
    break;
  default:
    pr_err("Cirrus AFE CALLBACK: ERROR: Invalid module parameter: %d",
           payload32[1]);
    return -EINVAL;
  }

  return 0;
}

static int msm_routing_crus_gb_enable(struct snd_kcontrol *kcontrol,
                                      struct snd_ctl_elem_value *ucontrol) {
  const int crus_set = ucontrol->value.integer.value[0];

  pr_debug("%s: crus_set = %d\n", __func__, crus_set);

  if (crus_set > 255) {
    pr_err("%s: Invalid entry\n", __func__);
    return -EINVAL;
  }

  switch (crus_set) {
  case 0:
    crus_enable.value = 0;
    crus_gb_enable = 0;
    break;
  case 1:
    crus_enable.value = 1;
    crus_gb_enable = 1;
    break;
  default:
    return -EINVAL;
  }

  crus_afe_set_param(cirrus_ff_port, CIRRUS_GB_FFPORT, CRUS_AFE_PARAM_ID_ENABLE,
                     sizeof(struct crus_single_data_t), (void *)&crus_enable);

  crus_afe_set_param(cirrus_fb_port, CIRRUS_GB_FBPORT, CRUS_AFE_PARAM_ID_ENABLE,
                     sizeof(struct crus_single_data_t), (void *)&crus_enable);

  return 0;
}

static int msm_routing_crus_gb_cfg(struct snd_kcontrol *kcontrol,
                                      struct snd_ctl_elem_value *ucontrol) {
  const int crus_set = ucontrol->value.integer.value[0];
  int ret;

  pr_debug("%s: crus_set = %d\n", __func__, crus_set);

  if (crus_set > 255) {
    pr_err("%s: Invalid entry\n", __func__);
    return -EINVAL;
  }

  switch (crus_set) {
  case 0:
    pr_debug("%s: getting current temp\n", __func__);
    ret = msm_cirrus_get_temp_cal();
    if (ret)
      pr_err("%s: failed to get current temp %d\n", __func__, ret);
    break;
  case 1:
    pr_debug("%s: setting temp calibration\n", __func__);
    ret = msm_cirrus_write_calibration_data(&g_cali);
    if (ret)
      pr_err("%s: failed to set temp calibration %d\n", __func__, ret);
    break;
  case 2:
    pr_debug("%s: setting Opalum config to Music\n", __func__);
    ret = msm_cirrus_config_opalum_music();
    if (ret)
      pr_err("%s: failed to set Opalum Music config %d\n", __func__, ret);
    break;
  case 3:
    pr_debug("%s: setting Opalum config to Voice\n", __func__);
    ret = msm_cirrus_config_opalum_voice();
    if (ret)
      pr_err("%s: failed to set Opalum Voice config %d\n", __func__, ret);
    break;
  default:
    return -EINVAL;
  }

  crus_gb_cfg = crus_set;

  return 0;
}

static int msm_routing_crus_gb_ext_cfg(struct snd_kcontrol *kcontrol,
                                      struct snd_ctl_elem_value *ucontrol) {
  const int crus_set = ucontrol->value.integer.value[0];
  int ret;

  pr_debug("%s: crus_set = %d\n", __func__, crus_set);

  if (crus_set > 255) {
    pr_err("%s: Invalid entry\n", __func__);
    return -EINVAL;
  }

  switch (crus_set) {
  case 0:
    pr_debug("%s: default, nothing to do\n", __func__);
    break;
  case 1:
    pr_debug("%s: flashing RX default config\n", __func__);
    ret = msm_cirrus_flash_rx_config();
    if (ret)
      pr_err("%s: failed to flash RX config %d\n", __func__, ret);
    break;
  case 2:
    pr_debug("%s: flashing TX new config\n", __func__);
    ret = msm_cirrus_flash_tx_config();
    if (ret)
      pr_err("%s: failed to flash TX config %d\n", __func__, ret);
    break;
  case 3:
    pr_debug("%s: flashing RX new config\n", __func__);
    ret = msm_cirrus_flash_rx_new_config();
    if (ret)
      pr_err("%s: failed to flash RX (new) config %d\n", __func__, ret);
    break;
  default:
    return -EINVAL;
  }

  crus_gb_ext_cfg = crus_set;

  return 0;
}

static int msm_routing_crus_gb_enable_get(struct snd_kcontrol *kcontrol,
                                          struct snd_ctl_elem_value *ucontrol) {
  pr_debug("%s: crus_gb_enable = %d\n", __func__, crus_gb_enable);

  ucontrol->value.integer.value[0] = crus_gb_enable;

  return 0;
}

static int msm_routing_crus_gb_cfg_get(struct snd_kcontrol *kcontrol,
                                          struct snd_ctl_elem_value *ucontrol) {
  pr_debug("%s: crus_gb_cfg = %d\n", __func__, crus_gb_cfg);

  ucontrol->value.integer.value[0] = crus_gb_cfg;

  return 0;
}

static int msm_routing_crus_gb_ext_cfg_get(struct snd_kcontrol *kcontrol,
                                          struct snd_ctl_elem_value *ucontrol) {
  pr_debug("%s: crus_gb_ext_cfg = %d\n", __func__, crus_gb_ext_cfg);

  ucontrol->value.integer.value[0] = crus_gb_ext_cfg;

  return 0;
}

static const char *const crus_en_text[] = {"Disable", "Enable",};

static const char *const crus_cfg_text[] = {
  "Get Current Temp", 
  "Set Temp Calibration",
  "Opalum Music",
  "Opalum Voice",
};

static const char *const crus_ext_cfg_text[] = {
  "Default", 
  "RX Default",
  "TX New",
  "RX New",
};

static const struct soc_enum crus_en_enum[] = {
    SOC_ENUM_SINGLE_EXT(2, crus_en_text),
};

static const struct soc_enum crus_cfg_enum[] = {
    SOC_ENUM_SINGLE_EXT(4, crus_cfg_text),
};

static const struct soc_enum crus_ext_cfg_enum[] = {
    SOC_ENUM_SINGLE_EXT(4, crus_ext_cfg_text),
};

static const struct snd_kcontrol_new crus_mixer_controls[] = {
    SOC_ENUM_EXT("Cirrus GB Enable", crus_en_enum[0],
                 msm_routing_crus_gb_enable_get, msm_routing_crus_gb_enable),
    SOC_ENUM_EXT("Cirrus GB Config", crus_cfg_enum[0],
                 msm_routing_crus_gb_cfg_get, msm_routing_crus_gb_cfg),
    SOC_ENUM_EXT("Cirrus GB Ext Config", crus_ext_cfg_enum[0],
                 msm_routing_crus_gb_ext_cfg_get, msm_routing_crus_gb_ext_cfg),
};

void msm_crus_pb_add_controls(struct snd_soc_platform *platform) {
  crus_gb_device = platform->dev;

  if (crus_gb_device == NULL)
    pr_err("%s: platform->dev is NULL!\n", __func__);
  else
    pr_debug("%s: platform->dev = %lx\n", __func__,
            (unsigned long)crus_gb_device);

  snd_soc_add_platform_controls(platform, crus_mixer_controls, 3);
  return;
}

static struct crus_gb_cali_data msm_cirrus_get_speaker_calibration_data(void) {
  struct crus_gb_cali_data ret;
  char proinfo_data[30];
  struct file *fp;
  mm_segment_t old_fs;

  // Initialize default data
  ret.temp_acc = 285000;
  ret.count = 16;
  ret.ambient = 28;
  ret.ret = -EINVAL;

  old_fs = get_fs();
  set_fs(KERNEL_DS);

  msleep(50);

  fp = filp_open("/dev/block/bootdevice/by-name/proinfo", O_RDWR | O_CREAT,
                 0660);
  if (IS_ERR(fp)) {
    ret.ret = PTR_ERR(fp);
    printk("%s: Failed to open proinfo pseudo-file (ret = %d)\n", __func__,
           ret.ret);
    set_fs(old_fs);
    return ret;
  }

  fp->f_pos = SPK_CAL_DATA_START_ADDR;

  if (!fp->f_op->read)
    ret.ret = (int)vfs_read(fp, proinfo_data, SPK_CAL_DATA_LENGTH, &fp->f_pos);
  else
    ret.ret = fp->f_op->read(fp, proinfo_data, SPK_CAL_DATA_LENGTH, &fp->f_pos);

  filp_close(fp, NULL);
  set_fs(old_fs);

  if (ret.ret < 0) {
    pr_err("%s: Failed to read calibration data from proinfo pseudo-file (ret "
           "= %d)\n",
           __func__, ret.ret);
    return ret;
  }

  ret.ret =
      sscanf(proinfo_data, "%d,%d,%d", &ret.temp_acc, &ret.count, &ret.ambient);
  if (ret.ret != 3) {
    pr_err("%s: Failed to parse calibration data (ret = %d, data = %s)\n",
           __func__, ret.ret, proinfo_data);
    return ret;
  }

  // sscanf successful return code is 3,
  // so set return code to 0 if all is right
  ret.ret = 0;

  return ret;
}

static int msm_cirrus_flash_tx_config(void) {
  char *data;
  const struct firmware *fw;
  int ret = 0;

  ret = request_firmware(&fw, "crus_gb_config_new_tx.bin", crus_gb_device);
  if (ret != 0) {
    pr_err("%s: failed to load TX config: %d", __func__, ret);
    return ret;
  }

  data = kmalloc(fw->size, GFP_KERNEL);
  memcpy(data, fw->data, fw->size);
  data[fw->size] = '\0';
  pr_debug("%s: length = %d; data = %lx\n", __func__, (unsigned int)fw->size,
           (long unsigned int)fw->data);

  ret = crus_afe_send_config(data, CIRRUS_GB_FBPORT);
  pr_debug("%s: ret: %d", __func__, ret);

  release_firmware(fw);
  kfree(data);

  return 0;
}

static int msm_cirrus_flash_rx_new_config(void) {
  char *data;
  const struct firmware *fw;
  int ret = 0;

  ret = request_firmware(&fw, "crus_gb_config_new_rx.bin", crus_gb_device);
  if (ret != 0) {
    pr_err("%s: failed to load RX config: %d\n", __func__, ret);
    return ret;
  }

  data = kmalloc(fw->size, GFP_KERNEL);
  memcpy(data, fw->data, fw->size);
  data[fw->size] = '\0';
  pr_debug("%s: length = %d; data = %lx\n", __func__, (unsigned int)fw->size,
           (long unsigned int)fw->data);

  // Protection for double load
  if (!voice_config_loaded) {
    voice_config_loaded = 1;
    ret = crus_afe_send_config(data, CIRRUS_GB_FFPORT);
    pr_debug("%s: ret: %d\n", __func__, ret);
  } else {
    pr_debug("%s: skip config load\n", __func__);
  }

  release_firmware(fw);
  kfree(data);

  return 0;
}

static int msm_cirrus_flash_rx_config(void) {
  char *data;
  const struct firmware *fw;
  int ret = 0;

  ret = request_firmware(&fw, "crus_gb_config_default.bin", crus_gb_device);
  if (ret != 0) {
    pr_err("%s: failed to load RX config: %d\n", __func__, ret);
    return ret;
  }

  data = kmalloc(fw->size, GFP_KERNEL);
  memcpy(data, fw->data, fw->size);
  data[fw->size] = '\0';
  pr_debug("%s: length = %d; data = %lx\n", __func__, (unsigned int)fw->size,
           (long unsigned int)fw->data);

  // Protection for double load
  if (!music_config_loaded) {
    music_config_loaded = 1;
    ret = crus_afe_send_config(data, CIRRUS_GB_FFPORT);
    pr_debug("%s: ret: %d\n", __func__, ret);
  } else {
    pr_debug("%s: skip config load\n", __func__);
  }

  release_firmware(fw);
  kfree(data);

  return 0;
}

static int msm_cirrus_config_opalum_music(void) {
  struct crus_single_data_t opalum_ctl = {0};

  pr_debug("%s: setting Opalum to Music mode\n", __func__);

  music_config_loaded = 0;

  return crus_afe_set_param(cirrus_ff_port, CIRRUS_GB_FFPORT, CRUS_PARAM_OPALUM,
                            sizeof(struct crus_single_data_t),
                            (void *)&opalum_ctl);
}

static int msm_cirrus_config_opalum_voice(void) {
  struct crus_single_data_t opalum_ctl = {1};

  pr_debug("%s: setting Opalum to Voice mode\n", __func__);

  voice_config_loaded = 0;

  return crus_afe_set_param(cirrus_ff_port, CIRRUS_GB_FFPORT, CRUS_PARAM_OPALUM,
                            sizeof(struct crus_single_data_t),
                            (void *)&opalum_ctl);
}

static int msm_cirrus_write_calibration_data(struct crus_gb_cali_data *cali) {
  struct crus_triple_data_t cal_data = {
      cali->temp_acc,
      cali->count,
      cali->ambient,
  };

  pr_debug("%s: temp_acc: %d, count: %d, ambient: %d\n", __func__,
           cal_data.data1, cal_data.data2, cal_data.data3);

  return crus_afe_set_param(
      cirrus_ff_port, CIRRUS_GB_FFPORT, CRUS_PARAM_SET_TEMP_CAL,
      sizeof(struct crus_triple_data_t), (void *)&cal_data);
}

static int msm_cirrus_get_temp_cal(void) {
  struct crus_dual_data_t cal_data;
  int ret;

  ret = crus_afe_get_param(cirrus_ff_port, CIRRUS_GB_FFPORT,
                           CRUS_PARAM_RX_GET_TEMP,
                           sizeof(struct crus_dual_data_t), (void *)&cal_data);
  if (ret)
    return ret;

  pr_debug("%s: temp_acc: %d, count: %d\n", __func__, cal_data.data1,
           cal_data.data2);

  return 0;
}

void msm_cirrus_callback(void) {
  g_cali = msm_cirrus_get_speaker_calibration_data();
  if (g_cali.ret)
    pr_err("%s: WARNING: USING DEFAULT CALIBRATION VALUES\n", __func__);
}

static int __init crus_gb_init(void) {
  pr_info("%s: initializing\n", __func__);
  atomic_set(&crus_gb_get_param_flag, 0);
  atomic_set(&crus_gb_misc_usage_count, 0);
  mutex_init(&crus_gb_get_param_lock);
  mutex_init(&crus_gb_lock);

  return 0;
}
module_init(crus_gb_init);

static void __exit crus_gb_exit(void) {
  mutex_destroy(&crus_gb_get_param_lock);
  mutex_destroy(&crus_gb_lock);
}
module_exit(crus_gb_exit);
