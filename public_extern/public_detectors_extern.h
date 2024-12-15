/**********************************************************
filename   : public_detectors_extern.h
function   : 探测器接口定义（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#ifndef _PUBLIC_DETECTORS_EXTERN_H_
#define _PUBLIC_DETECTORS_EXTERN_H_

#include "common.h"
#include "public_detectors.h"

// 探测器类型
enum DETECTORS_EXTERN_TYPE{
    DECT_EXTERN_TYPE_BEGIN = DECT_TYPE_NUM - 1,  // <--!!这行不能删
    // TODO: 添加扩展类型
    DECT_TYPE_SENSOR_COUNTER,
    DECT_EXTERN_TYPE_NUM
};

/*
 * 启动、停止探测器处理
*/
ushort_t DetectorExternStart(ushort_t id, uint_t arg1);
ushort_t DetectorExternStop(ushort_t id);

/*
 * 探测位是否已到达
*/
ushort_t IsDetectorExternTriggered(ushort_t id);


#endif
