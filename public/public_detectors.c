/**********************************************************
filename   : public_detectors.c
function   : 探测器接口定义（公共部分）
author     : 谢令
created    : 2022-12-07
comment    :
**********************************************************/
#include "public_detectors_extern.h"
#include "public_photosensors.h"
#include "public_timers.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern const uint_t G_DETECTORS_NUMBER;            // 配置探测器数量
extern DETECTOR_CONFIG g_cfg_detector[];          // 配置探测器基础信息
//=================================================================

/*
 * 重置探测器
*/
void DetectorReset(ushort_t id){
}

/*
 * 启动、停止探测器处理
*/
ushort_t DetectorStart(ushort_t id, uint_t arg1){
    switch(g_cfg_detector[id].dectType){
        case DECT_TYPE_PASS:
        case DECT_TYPE_REJECT:
        case DECT_TYPE_PHOTOSENSOR:
        case DECT_TYPE_REVERSE_PHOTOSENSOR:{
            return 0;
        }
        case DECT_TYPE_TIMEOUT:{
            TimerStartCountDown(g_cfg_detector[id].referId, arg1);
            return 0;
        }
        default:{
            DetectorExternStart(id, arg1);
            break;
        }
    }
    return 0;
}

ushort_t DetectorStop(ushort_t id){
    switch(g_cfg_detector[id].dectType){
        case DECT_TYPE_PASS:
        case DECT_TYPE_REJECT:
        case DECT_TYPE_PHOTOSENSOR:
        case DECT_TYPE_REVERSE_PHOTOSENSOR:{
            return 0;
        }
        case DECT_TYPE_TIMEOUT:{
            TimerStop(g_cfg_detector[id].referId);
            return 0;
        }
        default:{
            DetectorExternStop(id);
            break;
        }
    }
    return 0;
}

/*
 * 探测位是否已到达
*/
ushort_t IsDetectorTriggered(ushort_t id){
    switch(g_cfg_detector[id].dectType){
        case DECT_TYPE_PASS:{
            return 1;
        }
        case DECT_TYPE_REJECT:{
            return 0;
        }
        case DECT_TYPE_PHOTOSENSOR:{
            return IsPhotoSensorActived(g_cfg_detector[id].referId);
        }
        case DECT_TYPE_REVERSE_PHOTOSENSOR:{
            if(IsPhotoSensorActived(g_cfg_detector[id].referId)){
                return 0;
            }
            return 1;
        }
        case DECT_TYPE_TIMEOUT:{
            return IsTimeOut(g_cfg_detector[id].referId);
        }
        default:{
            break;
        }
    }
    return IsDetectorExternTriggered(id);
}

