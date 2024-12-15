/**********************************************************
filename   : public_timers.c
function   : 计时器（公共部分）
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/

#include "public_timers.h"

extern uint_t G_TIMERS_NUMBER;
extern TIMER_CONFIG g_cfg_timer[];
extern TIMER_STATUS g_st_timer[];

// 根据计数器折算成毫秒(ms)时长
uint_t CalcTimeMS(time_ul_t start, time_ul_t end)
{
    return end - start;
}

void TimerStart(uint_t id)
{
    g_st_timer[id].tmStartTime = GetCurrentSysTickCounter();
    g_st_timer[id].tmStage = TIMER_RECORDING;
    g_st_timer[id].intervalCnt = 0;
    g_st_timer[id].tapCnt = 0;
}

// 启动计时器: 指定超时毫秒数
void TimerStartCountDown(uint_t id,uint_t ms){
    g_cfg_timer[id].time_ms = ms;
    TimerStart(id);
}

uint_t TimerElapsedMs(uint_t id){
    return CalcTimeMS(g_st_timer[id].tmStartTime, GetCurrentSysTickCounter());
}

// 计时器心跳处理
void TimerTick(uint_t id)
{
    if (TIMER_RECORDING == g_st_timer[id].tmStage){
        uint_t elapsed_ms = CalcTimeMS(g_st_timer[id].tmStartTime, GetCurrentSysTickCounter());
        if (TIMER_TYPE_INTERVAL == g_cfg_timer[id].type){
            g_st_timer[id].intervalCnt = elapsed_ms/g_cfg_timer[id].time_ms;
        }
        else{
            if (elapsed_ms >= g_cfg_timer[id].time_ms){
                g_st_timer[id].tmStage = TIMER_DONE;
            }
        }
    }
}

void TimerStop(uint_t id)
{
    g_st_timer[id].tmStartTime = 0;
    g_st_timer[id].tmStage = TIMER_READY;
}

int IsTimeOut(uint_t id)
{
    TimerTick(id);
    if (TIMER_DONE == g_st_timer[id].tmStage){
        return 1;
    }
    return 0;
}

int TimerTap(uint_t id)
{
    TimerTick(id);
    if (TIMER_RECORDING == g_st_timer[id].tmStage && TIMER_TYPE_INTERVAL == g_cfg_timer[id].type){
        uint_t elapsed = g_st_timer[id].intervalCnt - g_st_timer[id].tapCnt;
        if (elapsed > 0){
            g_st_timer[id].tapCnt = g_st_timer[id].intervalCnt;
            return elapsed;
        }
    }
    return 0;
}

void AllTimersTick(void)
{
    uint_t i = 0;
    for (i = 0; i < G_TIMERS_NUMBER; i++)
    {
        TimerTick(i);
    }
}

