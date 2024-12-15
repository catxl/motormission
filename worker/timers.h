/**********************************************************
filename   : timers.h
function   : 计时器
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/

#ifndef _WORKER_TIMERS_H_
#define _WORKER_TIMERS_H_
#include "public_timers.h"

enum TIMER_IDS {
    TIMER_LED_INTERVAL,
    TIMER_CLEAN_DELAY_INTERVAL,
    TIMER_SLEEP_1MS_INTERVAL,
    TIMER_DELAY_MOTOR_CLEAN_PLATE_ROTATE,                // 
    TIMER_DELAY_MOTOR_CLEANER_SUCKER_VERT,               // 
    TIMER_DELAY_MOTOR_SUBSTRATE_MIXER_VERT,              // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M01, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M02, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M03, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M04, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M05, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M06, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M07, // 
    TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M08, // 
    TIMER_DELAY_MOTOR_SUBSTRATE_A_PLUNGER,               // 
    TIMER_DELAY_MOTOR_SUBSTRATE_B_PLUNGER,               // 
    TIMER_DELAY_MOTOR_CLEANER_MIXER_VERT,                // 
    TIMER_DELAY_MOTOR_SUCK_LIQUID_PERSISTALTIC_PUMP,     // 
    TIMER_DELAY_DC_MOTOR_CLEANER_MIXER,                  // 
    TIMER_DELAY_DC_MOTOR_SUBSTRATE_MIXER,                // 
    TIMER_DELAY_SUBSTRATE_A_SPITTER,                     // 
    TIMER_DELAY_SUBSTRATE_B_SPITTER,                     // 
    TIMER_DELAY_MAGNETIC_SEPARATION,                     // 
    TIMERS_NUMBER
};

// configure systick
void systick_config(void);
// sleep(ms)
void delay_ms(uint_t ms);

#endif
