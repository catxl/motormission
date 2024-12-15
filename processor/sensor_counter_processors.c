/**********************************************************
filename   : sensor_counter_processors.c
function   : 传感器计数处理器头文件
author     : 谢令
created    : 2022-11-28
comment    :
**********************************************************/
#include "sensor_counter_processors.h"
#include "photosensors.h"

// 传感器计数处理器配置项
typedef struct
{
    ushort_t  uiPhotoSensorId;                   // 关联的传感器ID
} SENSOR_COUNTER_PROCESSOR_CONFIG;

// 传感器计数处理器状态值
typedef struct
{
    uchar_t ucStatus;               // 处理器工作状态
    uchar_t ucLastestSensorStatus;  // 前一次传感器状态
    uint_t uiRaiseCount;            // 上升沿累积数量
    uint_t uiDeclineCount;          // 下降沿累积数量
    uint_t uiHopCount;              // 跳变数量
    uint_t uiWantedHopCount;        // 期待的跳变数量
} SENSOR_COUNTER_PROCESSOR_STATUS;

static SENSOR_COUNTER_PROCESSOR_CONFIG s_cfg_sensor_counter_processor[SENSOR_COUNTER_PROCESSORS_NUMBER] = {
//  {uiPhotoSensorId}
    {PHOTOSENSOR_CLEAN_PLATE_ROTATE_COUNTER_S1}
};

static SENSOR_COUNTER_PROCESSOR_STATUS s_st_sensor_counter_processor[SENSOR_COUNTER_PROCESSORS_NUMBER] = {{0}};


/*
 * 重置传感器计数处理
*/
void SensorCounterProcessingReset(ushort_t id){
    s_st_sensor_counter_processor[id].ucStatus = SENSOR_COUNTER_PROC_READY;
    s_st_sensor_counter_processor[id].ucLastestSensorStatus = 0xFF;
    s_st_sensor_counter_processor[id].uiRaiseCount = 0;
    s_st_sensor_counter_processor[id].uiDeclineCount = 0;
    s_st_sensor_counter_processor[id].uiHopCount = 0;
}

/*
 * 启动、停止传感器计数处理
*/
ushort_t SensorCounterProcessingStart(ushort_t id, uint_t wantedHopCnt){
    SensorCounterProcessingReset(id);
    s_st_sensor_counter_processor[id].ucStatus = SENSOR_COUNTER_PROC_WORKING;
    s_st_sensor_counter_processor[id].uiWantedHopCount = wantedHopCnt;
    s_st_sensor_counter_processor[id].ucLastestSensorStatus = GetPhotoSensorStatus(s_cfg_sensor_counter_processor[id].uiPhotoSensorId);
    return 0;
}

ushort_t SensorCounterProcessingStop(ushort_t id){
    s_st_sensor_counter_processor[id].ucStatus = SENSOR_COUNTER_PROC_READY;
    return 0;
}

/*
 * 是否已达成目标
*/
ushort_t IsSensorCounterProcessingTriggered(ushort_t id){
    if(SENSOR_COUNTER_PROC_WORKING != s_st_sensor_counter_processor[id].ucStatus){
        return 0;
    }
    if(s_st_sensor_counter_processor[id].uiHopCount >= s_st_sensor_counter_processor[id].uiWantedHopCount){
        return 1;
    }
    return 0;
}

/*
 * 获取当前计数值
*/
uint_t SensorCounterGet(ushort_t id){
    return s_st_sensor_counter_processor[id].uiRaiseCount;
}

/*
 * 处理心跳: 更新数据
*/
ushort_t SensorCounterProcessingTick(ushort_t id){
    uchar_t current_sensor_status = 0;
    if(SENSOR_COUNTER_PROC_WORKING != s_st_sensor_counter_processor[id].ucStatus){
        return 0;
    }
    // 读取计数器对应的传感器的当前状态，如果遇到“上升沿”，则对应计数器+1
    current_sensor_status = GetPhotoSensorStatus(s_cfg_sensor_counter_processor[id].uiPhotoSensorId);
    if(PHOTOSENSOR_NOT_ACTIVED == s_st_sensor_counter_processor[id].ucLastestSensorStatus 
       && PHOTOSENSOR_ACTIVED == current_sensor_status){
        s_st_sensor_counter_processor[id].uiRaiseCount += 1;
        s_st_sensor_counter_processor[id].uiHopCount += 1;
    }
    else if(PHOTOSENSOR_ACTIVED == s_st_sensor_counter_processor[id].ucLastestSensorStatus 
       && PHOTOSENSOR_NOT_ACTIVED == current_sensor_status){
        s_st_sensor_counter_processor[id].uiDeclineCount += 1;
        s_st_sensor_counter_processor[id].uiHopCount += 1;
    }
    s_st_sensor_counter_processor[id].ucLastestSensorStatus = current_sensor_status;
    return 0;
}

/*
 * 所有传感器计数器心跳处理
*/
ushort_t SensorCounterProcessorsAllTick(void){
    uint_t i = 0;
    for(i=0; i<SENSOR_COUNTER_PROCESSORS_NUMBER; i++){
        SensorCounterProcessingTick(i);
    }
    return 0;
}
