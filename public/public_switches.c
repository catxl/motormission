/**********************************************************
filename   : public_switches.c
function   : 开关控制（公共部分）
author     : 谢令
created    : 2022-11-23
comment    :
**********************************************************/

#include "public_switches.h"


/*
 * 注意: FPGA层逻辑: 1关 / 0开
*/
#define FPGA_SWITCH_ON   0
#define FPGA_SWITCH_OFF  1
// turn on == 置0
#define SWITCH_BIT_AS_TURN_ON(var, bitselector)   (var) &= ~(bitselector)
// turn off == 置1
#define SWITCH_BIT_AS_TURN_OFF(var, bitselector)   (var) |= (bitselector)


//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern const uint_t G_SWITCHES_NUMBER;            // 开关的数量
extern const ushort_t G_REG_SWITCH_SR;            // SR寄存器地址
extern const uint_t g_uiSwitchRegNum;             // 配置寄存器数量
extern ushort_t g_usSwitchRegCfg[];               // 配置寄存器地址列表
extern ushort_t g_uiSwitchRegValue[];             // 存储开关状态寄存器值的内存
extern SWITCH_CONFIG g_cfg_swithes[];             // 配置开关的基础信息
extern SWITCH_STATUS g_switches_status[];         // 存储记录开关状态的内存
//=================================================================


uchar_t GetSwitchStatus(ushort_t id){
    return g_switches_status[id].ucCurrentStatus;
}

ushort_t InitAllSwitches(void){
    int i = 0;
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        ushort_t regidx = g_cfg_swithes[i].uiRegIndex;
        if(SWITCH_OFF==g_cfg_swithes[i].ucInitStatus){
            SWITCH_BIT_AS_TURN_OFF(g_uiSwitchRegValue[regidx], g_cfg_swithes[i].uiBitSelector);
        }
        else{
            SWITCH_BIT_AS_TURN_ON(g_uiSwitchRegValue[regidx], g_cfg_swithes[i].uiBitSelector);
        }
        g_switches_status[i].ucCurrentStatus = g_cfg_swithes[i].ucInitStatus;
    }
    for(i=0; i<g_uiSwitchRegNum; i++){
        FPGA(g_usSwitchRegCfg[i]) = g_uiSwitchRegValue[i];
    }
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    return 0;
}

ushort_t SwitchTurnOn(ushort_t id){
    ushort_t regidx = 0;
    regidx = g_cfg_swithes[id].uiRegIndex;
    SWITCH_BIT_AS_TURN_ON(g_uiSwitchRegValue[regidx], g_cfg_swithes[id].uiBitSelector);
    FPGA(g_usSwitchRegCfg[regidx]) = g_uiSwitchRegValue[ regidx ];
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    g_switches_status[id].ucCurrentStatus = SWITCH_ON;
    return 0;
}

ushort_t SwitchTurnOff(ushort_t id){
    ushort_t regidx = 0;
    regidx = g_cfg_swithes[id].uiRegIndex;
    SWITCH_BIT_AS_TURN_OFF(g_uiSwitchRegValue[regidx], g_cfg_swithes[id].uiBitSelector);
    FPGA(g_usSwitchRegCfg[regidx]) = g_uiSwitchRegValue[ regidx ];
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    g_switches_status[id].ucCurrentStatus = SWITCH_OFF;
    return 0;
}

ushort_t SwitchSet(ushort_t id, uchar_t status){
    if(SWITCH_OFF==status){
        return SwitchTurnOff(id);
    }
    return SwitchTurnOn(id);
}

ushort_t SwitchBatchTurnOn(ushort_t* id_arr, ushort_t num){
    int i = 0;
    for(i=0; i<num; i++){
        ushort_t regidx = g_cfg_swithes[id_arr[i]].uiRegIndex;
        SWITCH_BIT_AS_TURN_ON(g_uiSwitchRegValue[regidx], g_cfg_swithes[i].uiBitSelector);
    }
    for(i=0; i<g_uiSwitchRegNum; i++){
        FPGA(g_usSwitchRegCfg[i]) = g_uiSwitchRegValue[i];
    }
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        g_switches_status[i].ucCurrentStatus = SWITCH_ON;
    }
    return 0;
}

ushort_t SwitchBatchTurnOff(ushort_t* id_arr, ushort_t num){
    int i = 0;
    for(i=0; i<num; i++){
        ushort_t regidx = g_cfg_swithes[id_arr[i]].uiRegIndex;
        SWITCH_BIT_AS_TURN_OFF(g_uiSwitchRegValue[regidx], g_cfg_swithes[i].uiBitSelector);
    }
    for(i=0; i<g_uiSwitchRegNum; i++){
        FPGA(g_usSwitchRegCfg[i]) = g_uiSwitchRegValue[i];
    }
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        g_switches_status[i].ucCurrentStatus = SWITCH_OFF;
    }
    return 0;
}

ushort_t AllSwitchesTurnOn(void){
    int i = 0;
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        ushort_t regidx = g_cfg_swithes[i].uiRegIndex;
        SWITCH_BIT_AS_TURN_ON(g_uiSwitchRegValue[regidx], g_cfg_swithes[i].uiBitSelector);
    }
    for(i=0; i<g_uiSwitchRegNum; i++){
        FPGA(g_usSwitchRegCfg[i]) = g_uiSwitchRegValue[i];
    }
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        g_switches_status[i].ucCurrentStatus = SWITCH_ON;
    }
    return 0;
}

ushort_t AllSwitchesTurnOff(void){
    int i = 0;
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        ushort_t regidx = g_cfg_swithes[i].uiRegIndex;
        SWITCH_BIT_AS_TURN_OFF(g_uiSwitchRegValue[regidx], g_cfg_swithes[i].uiBitSelector);
    }
    for(i=0; i<g_uiSwitchRegNum; i++){
        FPGA(g_usSwitchRegCfg[i]) = g_uiSwitchRegValue[i];
    }
    FPGA(G_REG_SWITCH_SR) = 0;  // 启动FPGA输出
    for(i=0; i<G_SWITCHES_NUMBER; i++){
        g_switches_status[i].ucCurrentStatus = SWITCH_OFF;
    }
    return 0;
}
