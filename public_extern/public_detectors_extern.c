/**********************************************************
filename   : public_detectors_extern.c
function   : 探测器接口定义（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#include "public_detectors_extern.h"
#include "sensor_counter_processors.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern const uint_t G_DETECTORS_NUMBER;            // 配置探测器数量
extern DETECTOR_CONFIG g_cfg_detector[];          // 配置探测器基础信息
//=================================================================

/*
 * 启动、停止探测器处理
*/
ushort_t DetectorExternStart(ushort_t id, uint_t arg1){
    switch(g_cfg_detector[id].dectType){
        case DECT_TYPE_SENSOR_COUNTER:{
            SensorCounterProcessingStart(g_cfg_detector[id].referId, arg1);
            return 0;
        }
        default:{
            break;
        }
    }
    return 0;
}

ushort_t DetectorExternStop(ushort_t id){
    switch(g_cfg_detector[id].dectType){
        case DECT_TYPE_SENSOR_COUNTER:{
            return SensorCounterProcessingStop(g_cfg_detector[id].referId);
        }
        default:{
            break;
        }
    }
    return 0;
}

/*
 * 探测位是否已到达
*/
ushort_t IsDetectorExternTriggered(ushort_t id){
    switch(g_cfg_detector[id].dectType){
        case DECT_TYPE_SENSOR_COUNTER:{
            return IsSensorCounterProcessingTriggered(g_cfg_detector[id].referId);
        }
        default:{
            break;
        }
    }
    return 0;
}

