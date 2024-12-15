/**********************************************************
filename   : public_photosensors.h
function   : 光电传感器（公共部分）
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#ifndef _PUBLIC_PHOTOSENSORS_H_
#define _PUBLIC_PHOTOSENSORS_H_

#include "common.h"

enum PHOTOSENSOR_STATUS{
    PHOTOSENSOR_NOT_ACTIVED = 0,          // 未激活（无遮挡）
    PHOTOSENSOR_ACTIVED                   // 激活（被遮挡）
};

// 光耦配置项
typedef struct
{
    ushort_t  uiRegIndex;         // 关联的寄存器下标
    ushort_t  uiBitSelector;      // 位选择器
} BI_PHOTOSENSOR_CONFIG;

// 光耦状态值
typedef struct
{
    uchar_t ucCurrentStatus;    // 当前状态
} BI_PHOTOSENSOR_STATUS;

/*
 * 读取所有光耦状态
*/
ushort_t ReadAllPhotoSensorStatus(void);

/*
 * 根据最近一次 ReadAllPhotoSensorStatus 调用后的结果，获取指定光耦状态
*/
uchar_t GetPhotoSensorStatus(uint_t id);
uchar_t IsPhotoSensorActived(uint_t id);

#endif
