/**********************************************************
filename   : photosensors.c
function   : 光电传感器
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#include "photosensors.h"

#define REG_SENSOR 0xE300


//=================================================================
/*
 * 本单元使用到的光耦的实现定义
*/
const uint_t G_PHOTOSENSORS_NUMBER = PHOTOSENSORS_NUMBER;            // 配置光耦数量
const uint_t g_uiRegNum = 2;                                         // 配置寄存器数量
ushort_t g_uiRegAddr[g_uiRegNum] = {                                 // 配置寄存器地址
    REG_SENSOR,  REG_SENSOR+1
};
ushort_t  g_uiRegValue[g_uiRegNum] = {0x0000, 0x0000};       // 存储寄存器值的内存
BI_PHOTOSENSOR_CONFIG g_cfg_photosensor[PHOTOSENSORS_NUMBER] = {     // 配置光耦基础信息
//                                       uiRegIndex,       uiBitSelector
//                                       关联的寄存器下标   位选择器
/* CLEAN_PLATE_ROTATE_POS0_S0,         */{0,               1},      // 
/* CLEAN_PLATE_ROTATE_COUNTER_S1,      */{0,               1<<1},   // 
/* CLEAN_PLATE_LIFT_RESET_S2,          */{0,               1<<2},   // 
/* CLEAN_PLATE_MIX_LIFT_HIGH_POS_S3,   */{0,               1<<3},   // 
/* CLEAN_PLATE_MIX_LIFT_LOW_POS_S4,    */{0,               1<<4},   // 
/* CLEAN_PLATE_MIX_SPEED_MEASURE_S5,   */{0,               1<<5},   // 
/* SUBSTRATE_MIX_LIFT_LOW_POS_S6,      */{0,               1<<6},   // 
/* SUBSTRATE_MIX_SPEED_MEASURE_S7,     */{0,               1<<7},   // 
/* SUBSTRATE_A1_LOC_S8,                */{0,               1<<8},   // 
/* SUBSTRATE_A2_LOC_S9,                */{0,               1<<9},   // 
/* SUBSTRATE_B1_LOC_S10,               */{0,               1<<10},  // 
/* SUBSTRATE_B2_LOC_S11,               */{0,               1<<11},  // 
/* SUBSTRATE_LIGHT_STRIP_1_S12,        */{0,               1<<12},  // 
/* SUBSTRATE_LIGHT_STRIP_2_S13,        */{0,               1<<13},  // 
/* SUBSTRATE_A_PLUNGER_PUMP_S14,       */{1,               1},      // 
/* SUBSTRATE_B_PLUNGER_PUMP_S15,       */{1,               1<<1}    // 
};
BI_PHOTOSENSOR_STATUS g_photosensor_status[PHOTOSENSORS_NUMBER] = {0};  // 存储光耦状态的内存
//=================================================================
