/**********************************************************
filename   : switches.c
function   : 开关控制头文件
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/
#include "switches.h"

#define REG_SWITCH_ADDR 0xE901   // 基地址
#define REG_SWITCH_SR   0xE900   // 输出地址


//=================================================================
/*
 * 本单元使用到的开关的实现定义
*/
const uint_t G_SWITCHES_NUMBER = SWITCHES_NUMBER;                  // 开关的数量
const ushort_t G_REG_SWITCH_SR = REG_SWITCH_SR;                    // SR寄存器地址
const uint_t g_uiSwitchRegNum = 1;                                 // 配置寄存器数量
ushort_t g_usSwitchRegCfg[g_uiSwitchRegNum] = {REG_SWITCH_ADDR};   // 配置寄存器地址列表
ushort_t g_uiSwitchRegValue[g_uiSwitchRegNum] = {0xFFFF};          // 存储阀状态寄存器值的内存
SWITCH_CONFIG g_cfg_swithes[SWITCHES_NUMBER] = {                   // 配置阀的基础信息
//  uiRegIndex,       uiBitSelector,     ucInitStatus
//  关联的寄存器下标   位选择器            初始状态     
    {0,               1,                 SWITCH_OFF},
    {0,               1<<1,              SWITCH_OFF},
    {0,               1,                 SWITCH_OFF},
    {0,               1<<1,              SWITCH_OFF}
};
SWITCH_STATUS g_switches_status[SWITCHES_NUMBER] = {0};      // 存储记录阀状态的内存
//=================================================================



