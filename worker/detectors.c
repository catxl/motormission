/**********************************************************
filename   : detectors.h
function   : 探测器
author     : 谢令
created    : 2022-12-07
comment    :
**********************************************************/
#include "detectors.h"
#include "photosensors.h"
#include "sensor_counter_processors.h"

// 旋转脉冲计数器，孔位数
static uint_t s_plate_rotate_sensor_counter_args[] = {72};

//=================================================================
/*
 * 本单元使用到的探测器的实现定义
*/
const uint_t G_DETECTORS_NUMBER = DETECTORS_NUMBER;         // 探测器数量
DETECTOR_CONFIG g_cfg_detector[DETECTORS_NUMBER] = {        // 配置混匀电机基础信息
                                                            //dectType,                      referId,                                         pDectArgs
                                                            //探测器类型                      关联的ID
/*DETECTOR_PASS,                                         */ {DECT_TYPE_PASS,                 0,                                             0}, // True探测器
/*DETECTOR_REJECT,                                       */ {DECT_TYPE_REJECT,               0,                                             0}, // False探测器
/*DETECTOR_PHOTOSENSOR_CLEAN_PLATE_ROTATE_POS0_S0,       */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_CLEAN_PLATE_ROTATE_POS0_S0,        0}, // 
/*DETECTOR_PHOTOSENSOR_CLEAN_PLATE_ROTATE_COUNTER_S1,    */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_CLEAN_PLATE_ROTATE_COUNTER_S1,     0}, // 
/*DETECTOR_PHOTOSENSOR_CLEANER_SUCKER_VERT_RESET_S2,     */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_CLEANER_SUCKER_VERT_RESET_S2,      0}, // 
/*DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_HIGH_POS_S3,   */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_CLEANER_MIXER_VERT_HIGH_POS_S3,    0}, // 
/*DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_LOW_POS_S4,    */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_CLEANER_MIXER_VERT_LOW_POS_S4,     0}, // 
/*DETECTOR_PHOTOSENSOR_CLEAN_PLATE_MIX_SPEED_MEASURE_S5, */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_CLEAN_PLATE_MIX_SPEED_MEASURE_S5,  0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_MIXER_VERT_LOW_POS_S6,  */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_MIXER_VERT_LOW_POS_S6,   0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_MIX_SPEED_MEASURE_S7,   */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_MIX_SPEED_MEASURE_S7,    0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_A1_LOC_S8,              */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_A1_LOC_S8,               0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_A2_LOC_S9,              */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_A2_LOC_S9,               0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_B1_LOC_S10,             */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_B1_LOC_S10,              0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_B2_LOC_S11,             */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_B2_LOC_S11,              0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_1_S12,      */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_1_S12,       0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_2_S13,      */ {DECT_TYPE_PHOTOSENSOR,          PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_2_S13,       0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_A_PLUNGER_S14,          */ {DECT_TYPE_REVERSE_PHOTOSENSOR,  PHOTOSENSOR_SUBSTRATE_A_PLUNGER_S14,           0}, // 
/*DETECTOR_PHOTOSENSOR_SUBSTRATE_B_PLUNGER_S15,          */ {DECT_TYPE_REVERSE_PHOTOSENSOR,  PHOTOSENSOR_SUBSTRATE_B_PLUNGER_S15,           0}, // 
/*DETECTOR_MOTOR_PLATE_ROTATE_PULSE_COUNTER,             */ {DECT_TYPE_SENSOR_COUNTER,       SENSOR_COUNTER_CLEAN_PLATE_ROTATE,             s_plate_rotate_sensor_counter_args}, // 电机编码器探测器：旋转电机编码器
};
//=================================================================
