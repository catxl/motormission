/**********************************************************
filename   : valves.c
function   : 阀驱动控制头文件
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#include "valves.h"


#define REG_VALVE_ADDR 0xE501 // 阀基地址
#define REG_VALVE_SR   0xE500   // 阀输出地址

//=================================================================
/*
 * 本单元使用到的阀的实现定义
*/
const uint_t G_VALVES_NUMBER = VALVES_NUMBER;             // 阀的数量
const ushort_t G_REG_VALVE_SR = REG_VALVE_SR;             // SR寄存器地址
const uint_t g_uiValveRegNum = 1;                         // 配置寄存器数量
ushort_t g_sRegCfg[g_uiValveRegNum] = {REG_VALVE_ADDR};   // 配置寄存器地址列表
ushort_t g_uiValveRegValue[g_uiValveRegNum] = {0xFFFF};   // 存储阀状态寄存器值的内存
BI_VALVE_CONFIG g_cfg_valves[VALVES_NUMBER] = {           // 配置阀的基础信息
//                                        uiRegIndex,       uiBitSelector,     ucInitStatus
//                                        关联的寄存器下标   位选择器            初始状态     
/*VALVE_SUBSTRATE_A_EMIT_3WAY,          */{0,               1,                 VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_A_SUCTION_2WAY,       */{0,               1<<1,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_A_EMIT_2WAY,          */{0,               1<<2,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_B_EMIT_3WAY,          */{0,               1<<3,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_B_SUCTION_2WAY,       */{0,               1<<4,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_B_EMIT_2WAY,          */{0,               1<<5,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_A_TRANSFER_3WAY,      */{0,               1<<6,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_B_TRANSFER_3WAY,      */{0,               1<<7,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_A_EMIT_TRANSFER_3WAY, */{0,               1<<8,              VALVE_CLOSED}, 
/*VALVE_SUBSTRATE_B_EMIT_TRANSFER_3WAY, */{0,               1<<9,              VALVE_CLOSED}, 
/*VALVE_REACTION_WASTE_EMIT_PUMP_3WAY,  */{0,               1<<10,             VALVE_CLOSED}, 
/*VALVE_DETECTION_WASTE_EMIT_PUMP,      */{0,               1<<11,             VALVE_CLOSED}, 
/*VALVE_REAGENT_PLATE_COLDER_EMIT_PUMP, */{0,               1<<12,             VALVE_CLOSED}  
};
BI_VALVE_STATUS g_valve_status[VALVES_NUMBER] = {0};      // 存储记录阀状态的内存
//=================================================================

