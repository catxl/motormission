/**********************************************************
filename   : sensor_counter_processors.h
function   : 传感器计数处理器头文件
author     : 谢令
created    : 2022-11-28
comment    :
**********************************************************/
#ifndef _SENSOR_COUNTER_PROCESSORS_H_
#define _SENSOR_COUNTER_PROCESSORS_H_

#include "common.h"

/*
* 传感器计数处理器标识（对象）定义
*/
enum SENSOR_COUNTER_PROCESSOR_IDS
{
    SENSOR_COUNTER_CLEAN_PLATE_ROTATE,           // 清洗盘旋转计数处理器
    SENSOR_COUNTER_PROCESSORS_NUMBER             // = 1
};

enum SENSOR_COUNTER_PROCESSOR_STATUS{
    SENSOR_COUNTER_PROC_READY = 0,               // 未未启动/未激活
    SENSOR_COUNTER_PROC_WORKING,                 // 数据累积中
};

/*
 * 重置传感器计数处理
*/
void SensorCounterProcessingReset(ushort_t id);

/*
 * 启动、停止传感器计数处理
*/
ushort_t SensorCounterProcessingStart(ushort_t id, uint_t wantedHopCnt);
ushort_t SensorCounterProcessingStop(ushort_t id);

/*
 * 处理心跳: 更新数据
*/
ushort_t SensorCounterProcessingTick(ushort_t id);

/*
 * 是否已达成目标
*/
ushort_t IsSensorCounterProcessingTriggered(ushort_t id);

/*
 * 所有传感器计数器心跳处理
*/
ushort_t SensorCounterProcessorsAllTick(void);

/*
 * 获取当前计数值
*/
uint_t SensorCounterGet(ushort_t id);

#endif
