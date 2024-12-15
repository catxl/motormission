/**********************************************************
filename   : detectors.h
function   : 探测器
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#ifndef _WORKER_DETECTORS_H_
#define _WORKER_DETECTORS_H_

#include "public_detectors_extern.h"

/*
* 探测器标识（对象）定义
*/
enum DETECTOR_IDS
{
    DETECTOR_PASS,                                             // True探测器
    DETECTOR_REJECT,                                           // False探测器
    DETECTOR_PHOTOSENSOR_CLEAN_PLATE_ROTATE_POS0_S0,           // 
    DETECTOR_PHOTOSENSOR_CLEAN_PLATE_ROTATE_COUNTER_S1,        // 
    DETECTOR_PHOTOSENSOR_CLEANER_SUCKER_VERT_RESET_S2,         // 
    DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_HIGH_POS_S3,       // 
    DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_LOW_POS_S4,        // 
    DETECTOR_PHOTOSENSOR_CLEAN_PLATE_MIX_SPEED_MEASURE_S5,     // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_MIXER_VERT_LOW_POS_S6,      // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_MIX_SPEED_MEASURE_S7,       // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_A1_LOC_S8,                  // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_A2_LOC_S9,                  // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_B1_LOC_S10,                 // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_B2_LOC_S11,                 // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_1_S12,          // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_2_S13,          // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_A_PLUNGER_S14,              // 
    DETECTOR_PHOTOSENSOR_SUBSTRATE_B_PLUNGER_S15,              // 
    DETECTOR_MOTOR_PLATE_ROTATE_PULSE_COUNTER,                 // 
    DETECTORS_NUMBER                                           // = 19
};

#endif
