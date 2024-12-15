/**********************************************************
filename   : public_detectors.h
function   : 探测器接口定义（公共部分）
author     : 谢令
created    : 2022-12-07
comment    :
**********************************************************/
#ifndef _PUBLIC_DETECTORS_H_
#define _PUBLIC_DETECTORS_H_

#include "common.h"

// 探测器类型
enum DETECTORS_TYPE{
    DECT_TYPE_PASS,
    DECT_TYPE_REJECT,
    DECT_TYPE_PHOTOSENSOR,
    DECT_TYPE_REVERSE_PHOTOSENSOR,
    DECT_TYPE_TIMEOUT,
    DECT_TYPE_NUM
};

// 探测器配置项
typedef struct
{
    ushort_t  dectType;           // 探测器类型
    ushort_t  referId;            // 关联的ID
    uint_t*   pDectArgs;          // 探测器类型配置参数指针
} DETECTOR_CONFIG;

/*
 * 启动、停止探测器处理
*/
ushort_t DetectorStart(ushort_t id, uint_t arg1);
ushort_t DetectorStop(ushort_t id);

/*
 * 探测位是否已到达
*/
ushort_t IsDetectorTriggered(ushort_t id);


#endif
