/**********************************************************
filename   : public_dc_motors.h
function   : 直流电机接口定义（公共部分）
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#ifndef _PUBLIC_DC_MOTORS_H_
#define _PUBLIC_DC_MOTORS_H_

#include "common.h"

enum DC_MOTOR_STATUS{
    DC_MOTOR_FREE = 0,          // 空闲
    DC_MOTOR_WORKING,           // 工作中
};

// 混匀电机配置项
typedef struct
{
    ushort_t  uiRegBaseAddr;         // 寄存器地址
    ushort_t  uiSpeedLevel;          // 混匀速度
} DC_MOTOR_CONFIG;

// 混匀电机状态值
typedef struct
{
    uchar_t ucCurrentStatus;    // 当前状态
} DC_MOTOR_STATUS;

/*
 * 启动、停止混匀电机
*/
ushort_t StartDcMotor(ushort_t id);
ushort_t StartDcMotorWithSpeed(ushort_t id, ushort_t speed);
ushort_t StopDcMotor(ushort_t id);


#endif
