/**********************************************************
filename   : switches.h
function   : 开关控制头文件
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/
#ifndef _WORKER_SWITCHES_H_
#define _WORKER_SWITCHES_H_

#include "public_switches.h"

/*
* 本单元开关标识（对象）定义
*/
enum SWITCH_IDS
{
    SWITCH_SUBSTRATE_1_LIGHT_STRIP_INDICATOR,  // 
    SWITCH_SUBSTRATE_2_LIGHT_STRIP_INDICATOR,  // 
    SWITCH_SUBSTRATE_BOTTLE_1_INDICATOR,       // 
    SWITCH_SUBSTRATE_BOTTLE_2_INDICATOR,       // 
    SWITCHES_NUMBER                            // = 4
};

#endif
