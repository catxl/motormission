/**********************************************************
filename   : public_switches.h
function   : 开关控制（公共部分）
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/
#ifndef _PUBLIC_SWITCHES_H_
#define _PUBLIC_SWITCHES_H_

#include "common.h"

enum SWITCH_TAG{
    SWITCH_OFF = 0,      // 关闭
    SWITCH_ON            // 打开
};

// 开关配置项
typedef struct
{
    ushort_t  uiRegIndex;         // 关联的寄存器下标
    ushort_t  uiBitSelector;      // 位选择器
    uchar_t ucInitStatus;         // 初始状态
} SWITCH_CONFIG;

// 开关状态值
typedef struct
{
    uchar_t ucCurrentStatus;    // 当前状态
} SWITCH_STATUS;

// 获取单个开关的状态
uchar_t GetSwitchStatus(ushort_t id);
// 初始化所有开关
ushort_t InitAllSwitches(void);
// 打开/关闭单个开关
ushort_t SwitchTurnOn(ushort_t id);
ushort_t SwitchTurnOff(ushort_t id);
ushort_t SwitchSet(ushort_t id, uchar_t status);

// 同时批量 打开/关闭 多个开关
ushort_t SwitchSwitchTurnOn(ushort_t* id_arr, ushort_t num);
ushort_t SwitchSwitchTurnOff(ushort_t* id_arr, ushort_t num);

// 同时批量 打开/关闭 所有开关
ushort_t AllSwitchesTurnOn(void);
ushort_t AllSwitchesTurnOff(void);



#endif
