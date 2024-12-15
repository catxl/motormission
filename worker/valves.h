/**********************************************************
filename   : valves.h
function   : 阀驱动控制头文件
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#ifndef _WORKER_VALVES_H_
#define _WORKER_VALVES_H_

#include "public_valves.h"

/*
* 本单元阀标识（对象）定义
*/
enum VALVE_IDS
{
    VALVE_SUBSTRATE_A_EMIT_3WAY,           // 
    VALVE_SUBSTRATE_A_SUCTION_2WAY,        // 
    VALVE_SUBSTRATE_A_EMIT_2WAY,           // 
    VALVE_SUBSTRATE_B_EMIT_3WAY,           // 
    VALVE_SUBSTRATE_B_SUCTION_2WAY,        // 
    VALVE_SUBSTRATE_B_EMIT_2WAY,           // 
    VALVE_SUBSTRATE_A_TRANSFER_3WAY,       // 
    VALVE_SUBSTRATE_B_TRANSFER_3WAY,       // 
    VALVE_SUBSTRATE_A_EMIT_TRANSFER_3WAY,  // 
    VALVE_SUBSTRATE_B_EMIT_TRANSFER_3WAY,  // 
    VALVE_REACTION_WASTE_EMIT_PUMP_3WAY,   // 
    VALVE_DETECTION_WASTE_EMIT_PUMP,       // 
    VALVE_REAGENT_PLATE_COLDER_EMIT_PUMP,  // 
    VALVES_NUMBER                          // = 12
};

#endif
