/**********************************************************
filename   : photosensors.h
function   : 光电传感器
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#ifndef _WORKER_PHOTOSENSORS_H_
#define _WORKER_PHOTOSENSORS_H_

#include "public_photosensors.h"

/*
* 光耦标识（对象）定义
*/
enum PHOTOSENSORS_IDS
{
    PHOTOSENSOR_CLEAN_PLATE_ROTATE_POS0_S0,        // 
    PHOTOSENSOR_CLEAN_PLATE_ROTATE_COUNTER_S1,     // 
    PHOTOSENSOR_CLEANER_SUCKER_VERT_RESET_S2,      // 
    PHOTOSENSOR_CLEANER_MIXER_VERT_HIGH_POS_S3,    // 
    PHOTOSENSOR_CLEANER_MIXER_VERT_LOW_POS_S4,     // 
    PHOTOSENSOR_CLEAN_PLATE_MIX_SPEED_MEASURE_S5,  // 
    PHOTOSENSOR_SUBSTRATE_MIXER_VERT_LOW_POS_S6,   // 
    PHOTOSENSOR_SUBSTRATE_MIX_SPEED_MEASURE_S7,    // 
    PHOTOSENSOR_SUBSTRATE_A1_LOC_S8,               // 
    PHOTOSENSOR_SUBSTRATE_A2_LOC_S9,               // 
    PHOTOSENSOR_SUBSTRATE_B1_LOC_S10,              // 
    PHOTOSENSOR_SUBSTRATE_B2_LOC_S11,              // 
    PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_1_S12,       // 
    PHOTOSENSOR_SUBSTRATE_LIGHT_STRIP_2_S13,       // 
    PHOTOSENSOR_SUBSTRATE_A_PLUNGER_S14,           // 
    PHOTOSENSOR_SUBSTRATE_B_PLUNGER_S15,           // 
    PHOTOSENSORS_NUMBER                            // = 16
};

#endif
