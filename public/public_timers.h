/**********************************************************
filename   : public_timers.h
function   : 计时器（公共部分）
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/

#ifndef _PUBLIC_TIMERS_H_
#define _PUBLIC_TIMERS_H_
#include "common.h"


time_ul_t GetCurrentSysTickCounter(void);

// 定义计时器类型：间隔、超时
enum TIMER_RECORD_TYPE {
    TIMER_TYPE_INTERVAL,//间隔
    TIMER_TYPE_TIMEOUT //超时
};

// 计时器状态
enum TIMER_RECORD_STAGE {
    TIMER_READY,
    TIMER_RECORDING,
    TIMER_DONE
};

// 计时器配置：ID、类型、时长(ms)
typedef struct{
    uint_t timerId;
    uint_t type;     // 间隔、超时
    uint_t time_ms;
}TIMER_CONFIG;

// 计时器运行状态值
typedef struct{
    time_ul_t tmStartTime;  // 起始时刻
    uint_t tmStage;         // 运行阶段
    uint_t intervalCnt;     // 适用TIMER_TYPE_INTERVAL类型: 当前时刻距离起始时刻经过的间隔(interval)数量
    uint_t tapCnt;          // 适用TIMER_TYPE_INTERVAL类型: 记录最后一次Tap时的intervalCnt
}TIMER_STATUS;

// 启动计时器
void TimerStart(uint_t id);
// 启动计时器: 指定超时毫秒数
void TimerStartCountDown(uint_t id, uint_t ms);
// 计时器从启动到现在所经历过的毫秒(ms)数
uint_t TimerElapsedMs(uint_t id);
// 停止计时器
void TimerStop(uint_t id);
// 是否已超时
int IsTimeOut(uint_t id);
/*
 * Tap操作：适用TIMER_TYPE_INTERVAL类型
 * 对指定(id)的间隔定时器，返回未处理的interval的个数，并标记为已处理
*/
int TimerTap(uint_t id);

// 心跳处理
void TimerTick(uint_t id);
void AllTimersTick(void);

#endif
