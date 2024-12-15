/**********************************************************
filename   : timers.c
function   : 计时器
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/

#include "timers.h"
#include "gd32f30x.h"

const uint_t G_TIMERS_NUMBER = TIMERS_NUMBER;
TIMER_CONFIG g_cfg_timer[TIMERS_NUMBER] = {
    {TIMER_LED_INTERVAL,           TIMER_TYPE_INTERVAL,  500},       // LED
    {TIMER_CLEAN_DELAY_INTERVAL,   TIMER_TYPE_INTERVAL,  10},        // 
    {TIMER_SLEEP_1MS_INTERVAL,     TIMER_TYPE_INTERVAL,  1},         // 用于实现Delay/Sleep的计时器
    {TIMER_DELAY_MOTOR_CLEAN_PLATE_ROTATE,                 TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_CLEANER_SUCKER_VERT,                TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SUBSTRATE_MIXER_VERT,               TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M01,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M02,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M03,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M04,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M05,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M06,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M07,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M08,  TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SUBSTRATE_A_PLUNGER,                TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SUBSTRATE_B_PLUNGER,                TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_CLEANER_MIXER_VERT,                 TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MOTOR_SUCK_LIQUID_PERSISTALTIC_PUMP,      TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_DC_MOTOR_CLEANER_MIXER,                   TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_DC_MOTOR_SUBSTRATE_MIXER,                 TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_SUBSTRATE_A_SPITTER,                      TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_SUBSTRATE_B_SPITTER,                      TIMER_TYPE_TIMEOUT,   200}, // 
    {TIMER_DELAY_MAGNETIC_SEPARATION,                      TIMER_TYPE_TIMEOUT,   200}, // 
};

TIMER_STATUS g_st_timer[TIMERS_NUMBER] = {0};


void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void delay_ms(uint_t ms){
    TimerStart(TIMER_SLEEP_1MS_INTERVAL);
    while(TimerElapsedMs(TIMER_SLEEP_1MS_INTERVAL) < ms){}       // waiting....
    TimerStop(TIMER_SLEEP_1MS_INTERVAL);
}

