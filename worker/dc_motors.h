/**********************************************************
filename   : dc_motors.h
function   : 直流电机
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#ifndef _WORKER_DC_MOTORS_H_
#define _WORKER_DC_MOTORS_H_

#include "public_dc_motors.h"

/*
* 直流电机标识（对象）定义
*/
enum DC_MOTOR_IDS
{
    DC_MOTOR_CLEANER_MIXER_M1,        // 混匀电机
    DC_MOTOR_SUBSTRATE_MIXER_M2,      // 混匀电机
    DC_MOTORS_NUMBER                  // = 2
};


#endif
