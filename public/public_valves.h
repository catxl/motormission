/**********************************************************
filename   : public_valves.h
function   : 阀操作（公共部分）
author     : 谢令
created    : 2022-11-16
comment    :
**********************************************************/
#ifndef _PUBLIC_VALVEDRIVE_H_
#define _PUBLIC_VALVEDRIVE_H_

#include "common.h"

enum VALVE_STATUS{
    VALVE_CLOSED = 0,      // 阀关闭
    VALVE_OPEN             // 阀开启
};

// 阀、泵配置项
typedef struct
{
    ushort_t  uiRegIndex;         // 关联的寄存器下标
    ushort_t  uiBitSelector;      // 位选择器
    uchar_t ucInitStatus;         // 初始状态
} BI_VALVE_CONFIG;

// 阀、泵状态值
typedef struct
{
    uchar_t ucCurrentStatus;    // 当前状态
} BI_VALVE_STATUS;

// 获取单个阀的状态
uchar_t GetValveStatus(ushort_t valve_id);
// 初始化所有阀
ushort_t InitAllValves(void);
// 打开/关闭单个阀
ushort_t ValveTurnOn(ushort_t valve_id);
ushort_t ValveTurnOff(ushort_t valve_id);
ushort_t ValveSetOnOff(ushort_t valve_id, uchar_t status);

// 同时批量 打开/关闭 多个阀
ushort_t ValveBatchTurnOn(ushort_t* valve_id_arr, ushort_t num);
ushort_t ValveBatchTurnOff(ushort_t* valve_id_arr, ushort_t num);
ushort_t ValveBatchTurn(ushort_t* valve_id_arr, ushort_t* status_arr, ushort_t num);

// 同时批量 打开/关闭 所有阀
ushort_t AllValvesTurnOn(void);
ushort_t AllValvesTurnOff(void);


#endif
