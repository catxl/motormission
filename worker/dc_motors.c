/**********************************************************
filename   : dc_motors.c
function   : 直流电机
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#include "dc_motors.h"

#define REG_DC_MOTOR_A 0xE400
#define REG_DC_MOTOR_B 0xE401

//=================================================================
/*
 * 本单元使用到的混匀电机的实现定义
*/
const uint_t G_DC_MOTORS_NUMBER = DC_MOTORS_NUMBER;              // 配置直流混匀电机数量
DC_MOTOR_CONFIG g_cfg_dc_motor[DC_MOTORS_NUMBER] = {             // 配置直流混匀电机基础信息
    //uiRegBaseAddr,         uiSpeedLevel
    //寄存器地址              混匀速度
    {REG_DC_MOTOR_A,         0x5000},
    {REG_DC_MOTOR_B,         0x5800}
};
DC_MOTOR_STATUS g_st_dc_motor[DC_MOTORS_NUMBER] = {             // 存储直流混匀电机状态的内存
    {DC_MOTOR_FREE}, {DC_MOTOR_FREE}
};
//=================================================================
