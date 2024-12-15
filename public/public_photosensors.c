/**********************************************************
filename   : public_photosensors.c
function   : 光电传感器（公共部分）
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#include "public_photosensors.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern const uint_t G_PHOTOSENSORS_NUMBER;            // 配置光耦数量
extern const uint_t g_uiRegNum;                       // 配置寄存器数量
extern ushort_t g_uiRegAddr[];                        // 配置寄存器地址
extern ushort_t  g_uiRegValue[];                      // 存储寄存器值的内存
extern BI_PHOTOSENSOR_CONFIG g_cfg_photosensor[];     // 配置光耦基础信息
extern BI_PHOTOSENSOR_STATUS g_photosensor_status[];  // 存储光耦状态的内存
//=================================================================

/*
 * 读取所有光耦状态
*/
ushort_t ReadAllPhotoSensorStatus(void){
    ushort_t i = 0;
    for(i=0; i<g_uiRegNum; i++){
        g_uiRegValue[i] = ~(FPGA(g_uiRegAddr[i]));
    }
    for(i=0; i<G_PHOTOSENSORS_NUMBER; i++){
        if(g_uiRegValue[g_cfg_photosensor[i].uiRegIndex] & g_cfg_photosensor[i].uiBitSelector){
            // 如果对应 bit == 1（激活/有遮挡）
            g_photosensor_status[i].ucCurrentStatus = PHOTOSENSOR_ACTIVED;
        }
        else{
            g_photosensor_status[i].ucCurrentStatus = PHOTOSENSOR_NOT_ACTIVED;
        }
    }
    return 0;
}

/*
 * 根据最近一次 ReadAllPhotoSensorStatus 调用后的结果，获取指定光耦状态
*/
uchar_t GetPhotoSensorStatus(uint_t id){
    return g_photosensor_status[id].ucCurrentStatus;
}
uchar_t IsPhotoSensorActived(uint_t id){
    return g_photosensor_status[id].ucCurrentStatus;
}
