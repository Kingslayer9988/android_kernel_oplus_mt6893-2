/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2016 MediaTek Inc.
 */

#ifndef __CCU_INTERFACE__
#define __CCU_INTERFACE__

#include "ccu_ext_interface/ccu_types.h"

extern MBOOL sec_vsync_pushed;

/******************************************************************************
* Task definition
******************************************************************************/
typedef enum {
	DELIMITER_SYSCTRL_MSG_MIN = 0, /*To identify ccu control msg count*/
	/*Receive by CCU*/
	MSG_TO_CCU_IDLE = DELIMITER_SYSCTRL_MSG_MIN,
	MSG_TO_CCU_SUSPEND, /*Request CCU to suspend on corresponding TG*/
	MSG_TO_CCU_RESUME, /*Request CCU to resume on corresponding TG*/
	MSG_TO_CCU_SHUTDOWN, /*Request CCU to shutdown*/
	MSG_TO_CCU_UPDATE_TG_SENSOR_MAP,

	DELIMITER_SYSCTRL_MSG_1, /*To identify ccu control msg count*/
	/*CCU internal task generated in HW isr*/
	MSG_CCU_INTERNAL_VSYNC_SYS = DELIMITER_SYSCTRL_MSG_1,
	MSG_CCU_INTERNAL_P1_DONE_SYS,
	MSG_CCU_INTERNAL_PR_LOG,
	MSG_CCU_INTERNAL_FORCE_SHUTDOWN, /*Request CCU to shutdown*/

	DELIMITER_SYSCTRL_MSG_MAX /*To identify ccu control msg count*/
} ccu_msg_id;

typedef enum {
    MSG_TO_APMCU_FLUSH_LOG, //CCU Request APMCU to print out CCU logs
    MSG_TO_APMCU_CCU_ASSERT, //CCU inform APMCU that CCU ASSERT occurs
    MSG_TO_APMCU_CCU_WARNING, //CCU inform APMCU that CCU WARNING occurs
	MSG_TO_APMCU_CAM_AFO_i,
	MSG_TO_APMCU_CAM_AFO_ABORT = 255
} ccu_to_ap_msg_id;

enum ccu_feature_type {
    CCU_FEATURE_UNDEF = 0x0,
    CCU_FEATURE_MIN = 0x1,
    CCU_FEATURE_AE = CCU_FEATURE_MIN,
    CCU_FEATURE_AF,
    CCU_FEATURE_LTM,
    CCU_FEATURE_3ACTRL,
    CCU_FEATURE_SYSCTRL,
    CCU_FEATURE_MAX,
};

enum ccu_tg_info {
	CCU_CAM_TG_NONE = 0x0,
	CCU_CAM_TG_MIN  = 0x1,
	CCU_CAM_TG_1    = 0x1,
	CCU_CAM_TG_2    = 0x2,
	CCU_CAM_TG_3    = 0x3,
	CCU_CAM_TG_MAX
};

struct ccu_msg {
	enum ccu_feature_type feature_type;
	uint32_t msg_id;
	MUINT32 in_data_ptr;
	MUINT32 out_data_ptr;
	enum ccu_tg_info tg_info;
	uint32_t sensor_idx;
};

struct ccu_control_info {
	enum ccu_feature_type feature_type;
	uint32_t sensor_idx; //new
	uint32_t msg_id;
	uint64_t inDataPtr;
	uint32_t inDataSize;
	uint64_t outDataPtr;
	uint32_t outDataSize;
};

struct ap2ccu_ipc_t {
	MUINT32 write_cnt;
	MUINT32 read_cnt;
	struct ccu_msg msg;
	MBOOL ack;
};

struct exif_data_addrs_s {
	MUINT32 ae_algo_data_addr;
	MUINT32 ae_init_data_addr;
	MUINT32 ae_vsync_info_addr;
	MUINT32 aesync_algo_in_addr;
	MUINT32 aesync_algo_out_addr;
};

#define CCU_INSTANCE_CAPACITY 2

struct __aligned(8) shared_buf_map {
	/*** from CCU->APMCU ***/
	MUINT32 ipc_in_data_addr_ccu;
	MUINT32 ipc_out_data_addr_ccu;
	MUINT32 ipc_in_data_base_offset;
	MUINT32 ipc_out_data_base_offset;
	MUINT32 ipc_base_offset;

	struct exif_data_addrs_s exif_data_addrs[CCU_INSTANCE_CAPACITY];
	MUINT32 ae_shared_buf_mva;

	//>>>>>>> LTM datas
	MUINT32 ltm_shared_buf_mva;

	//AF datas
	MUINT32 af_shared_buf_mva;

	/*** from APMCU->CCU ***/
	MUINT32 bkdata_ddr_buf_mva;
};



/******************************************************************************
* Status definition
******************************************************************************/
#define CCU_STATUS_INIT_DONE              0xffff0000
#define CCU_STATUS_INIT_DONE_2            0xffff00a5

#endif