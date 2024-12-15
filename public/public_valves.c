/**********************************************************
filename   : public_valves.c
function   : 阀操作（公共部分）
author     : 谢令
created    : 2022-11-16
comment    :
**********************************************************/
#include "public_valves.h"
#include "common.h"

/*
 * 注意: FPGA层逻辑: 1关 / 0开
*/
#define FPGA_VALVE_ON   0
#define FPGA_VALVE_OFF  1
// turn on == 置0
#define VALVE_BIT_AS_TURN_ON(var, bitselector)   ((var) &= ~(bitselector))
// turn off == 置1
#define VALVE_BIT_AS_TURN_OFF(var, bitselector)  ((var) |= (bitselector))


//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern const uint_t G_VALVES_NUMBER;             // 阀的数量
extern const ushort_t G_REG_VALVE_SR;            // SR寄存器地址
extern const uint_t g_uiValveRegNum;             // 配置寄存器数量
extern ushort_t g_sRegCfg[];                     // 配置寄存器地址列表
extern ushort_t g_uiValveRegValue[];             // 存储阀状态寄存器值的内存
extern BI_VALVE_CONFIG g_cfg_valves[];           // 配置阀的基础信息
extern BI_VALVE_STATUS g_valve_status[];         // 存储记录阀状态的内存
//=================================================================


uchar_t GetValveStatus(ushort_t valve_id){
    return g_valve_status[valve_id].ucCurrentStatus;
}

// 等待阀控制器可用，返回: 0(ready), 1(busy)
ushort_t WaitForReady(){
    uchar_t  ucValveStatus;                       //阀状态寄存器
    do{
        ucValveStatus = FPGA(G_REG_VALVE_SR);
        //FPGA允许MCU写数据或超时，则退出
        if((ucValveStatus & 1) != 1 )
        {
            break;
        }
    }while(1);
    if( (ucValveStatus & 1) == 1 )
    {
        return 1;
    }
    return 0;
}

ushort_t InitAllValves(void){
    int i = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    for(i=0; i<G_VALVES_NUMBER; i++){
        ushort_t regidx = g_cfg_valves[i].uiRegIndex;
        if(VALVE_CLOSED==g_cfg_valves[i].ucInitStatus){
            VALVE_BIT_AS_TURN_OFF(g_uiValveRegValue[regidx], g_cfg_valves[i].uiBitSelector);
        }
        else{
            VALVE_BIT_AS_TURN_ON(g_uiValveRegValue[regidx], g_cfg_valves[i].uiBitSelector);
        }
        g_valve_status[i].ucCurrentStatus = g_cfg_valves[i].ucInitStatus;
    }
    for(i=0; i<g_uiValveRegNum; i++){
        FPGA(g_sRegCfg[i]) = g_uiValveRegValue[i];
    }
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    return 0;
}

ushort_t ValveTurnOn(ushort_t valve_id){
    ushort_t regidx = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    regidx = g_cfg_valves[valve_id].uiRegIndex;
    VALVE_BIT_AS_TURN_ON(g_uiValveRegValue[regidx], g_cfg_valves[valve_id].uiBitSelector);
    FPGA(g_sRegCfg[regidx]) = g_uiValveRegValue[ regidx ];
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    g_valve_status[valve_id].ucCurrentStatus = VALVE_OPEN;
    return 0;
}

ushort_t ValveTurnOff(ushort_t valve_id){
    ushort_t regidx = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    regidx = g_cfg_valves[valve_id].uiRegIndex;
    VALVE_BIT_AS_TURN_OFF(g_uiValveRegValue[regidx], g_cfg_valves[valve_id].uiBitSelector);
    FPGA(g_sRegCfg[regidx]) = g_uiValveRegValue[ regidx ];
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    g_valve_status[valve_id].ucCurrentStatus = VALVE_CLOSED;
    return 0;
}

ushort_t ValveSetOnOff(ushort_t valve_id, uchar_t status){
    if(VALVE_CLOSED==status){
        return ValveTurnOff(valve_id);
    }
    return ValveTurnOn(valve_id);
}

ushort_t ValveBatchTurnOn(ushort_t* valve_id_arr, ushort_t num){
    int i = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    for(i=0; i<num; i++){
        ushort_t regidx = g_cfg_valves[valve_id_arr[i]].uiRegIndex;
        VALVE_BIT_AS_TURN_ON(g_uiValveRegValue[regidx], g_cfg_valves[valve_id_arr[i]].uiBitSelector);
    }
    for(i=0; i<g_uiValveRegNum; i++){
        FPGA(g_sRegCfg[i]) = g_uiValveRegValue[i];
    }
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    for(i=0; i<num; i++){
        g_valve_status[valve_id_arr[i]].ucCurrentStatus = VALVE_OPEN;
    }
    return 0;
}

ushort_t ValveBatchTurnOff(ushort_t* valve_id_arr, ushort_t num){
    int i = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    for(i=0; i<num; i++){
        ushort_t regidx = g_cfg_valves[valve_id_arr[i]].uiRegIndex;
        VALVE_BIT_AS_TURN_OFF(g_uiValveRegValue[regidx], g_cfg_valves[valve_id_arr[i]].uiBitSelector);
    }
    for(i=0; i<g_uiValveRegNum; i++){
        FPGA(g_sRegCfg[i]) = g_uiValveRegValue[i];
    }
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    for(i=0; i<num; i++){
        g_valve_status[valve_id_arr[i]].ucCurrentStatus = VALVE_CLOSED;
    }
    return 0;
}

ushort_t ValveBatchTurn(ushort_t* valve_id_arr, ushort_t* status_arr, ushort_t num){
    int i = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    for(i=0; i<num; i++){
        ushort_t regidx = g_cfg_valves[valve_id_arr[i]].uiRegIndex;
        if(VALVE_CLOSED == status_arr[i]){
            VALVE_BIT_AS_TURN_OFF(g_uiValveRegValue[regidx], g_cfg_valves[i].uiBitSelector);
        }
        else if(VALVE_OPEN == status_arr[i]){
            VALVE_BIT_AS_TURN_ON(g_uiValveRegValue[regidx], g_cfg_valves[i].uiBitSelector);
        }
    }
    for(i=0; i<g_uiValveRegNum; i++){
        FPGA(g_sRegCfg[i]) = g_uiValveRegValue[i];
    }
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    for(i=0; i<num; i++){
        if(VALVE_CLOSED == status_arr[i]){
            g_valve_status[valve_id_arr[i]].ucCurrentStatus = VALVE_CLOSED;
        }
        else if(VALVE_OPEN == status_arr[i]){
            g_valve_status[valve_id_arr[i]].ucCurrentStatus = VALVE_OPEN;
        }
    }
    return 0;
}

ushort_t AllValvesTurnOn(void){
    int i = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    for(i=0; i<G_VALVES_NUMBER; i++){
        ushort_t regidx = g_cfg_valves[i].uiRegIndex;
        VALVE_BIT_AS_TURN_ON(g_uiValveRegValue[regidx], g_cfg_valves[i].uiBitSelector);
    }
    for(i=0; i<g_uiValveRegNum; i++){
        FPGA(g_sRegCfg[i]) = g_uiValveRegValue[i];
    }
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    for(i=0; i<G_VALVES_NUMBER; i++){
        g_valve_status[i].ucCurrentStatus = VALVE_OPEN;
    }
    return 0;
}

ushort_t AllValvesTurnOff(void){
    int i = 0;
    if(WaitForReady()!=0){
        return 1;
    }
    for(i=0; i<G_VALVES_NUMBER; i++){
        ushort_t regidx = g_cfg_valves[i].uiRegIndex;
        VALVE_BIT_AS_TURN_OFF(g_uiValveRegValue[regidx], g_cfg_valves[i].uiBitSelector);
    }
    for(i=0; i<g_uiValveRegNum; i++){
        FPGA(g_sRegCfg[i]) = g_uiValveRegValue[i];
    }
    FPGA(G_REG_VALVE_SR) = 0;  // 启动FPGA输出
    for(i=0; i<G_VALVES_NUMBER; i++){
        g_valve_status[i].ucCurrentStatus = VALVE_CLOSED;
    }
    return 0;
}
