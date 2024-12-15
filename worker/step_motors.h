/**********************************************************
filename   : step_motors.h
function   : 步进电机
author     : 谢令
created    : 2022-11-03
comment    :
**********************************************************/
#ifndef _WORKER_STEP_MOTORS_H_
#define _WORKER_STEP_MOTORS_H_

#include "public_step_motors.h"

/*
 * 步进电机标识（对象）定义
 */
enum STEPPING_MOTOR_IDS
{
    STEP_MOTOR_CLEAN_PLATE_ROTATE_M0,             // 
    STEP_MOTOR_CLEANER_SUCKER_VERT_M1,            // 
    STEP_MOTOR_SUBSTRATE_MIXER_VERT_M2,           // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M01, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M02, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M03, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M04, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M05, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M06, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M07, // 
    STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M08, // 
    STEP_MOTOR_SUBSTRATE_A_PLUNGER_M11,           // 
    STEP_MOTOR_SUBSTRATE_B_PLUNGER_M12,           // 
    STEP_MOTOR_CLEANER_MIXER_VERT_M13,            // 
    STEP_MOTOR_SUCK_LIQUID_PERSISTALTIC_PUMP_M14, // 
    MOTOR_NUMBER                                  // = 15
};

#endif
