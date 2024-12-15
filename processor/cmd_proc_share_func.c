/**********************************************************
filename   : cmd_proc_share_func.c
function   : 命令处理函数(通用部分)
author     : 谢令
created    : 2022-11-21
comment    :
**********************************************************/
#include "cmd_proc_share_func.h"
#include "protocol.h"
#include "commpacket.h"
#include "can_io.h"
#include "usart_io.h"
#include "utils.h"
#include "all_missions.h"

#include "photosensors.h"
#include "step_motors.h"
#include "dc_motors.h"
#include "common_motors.h"
#include "valves.h"

#include "errcode.h"
#include "byte_sequence.h"

typedef int (*CMD_PROC_FUNC)(uchar_t* pPacket);

//0x9001  心跳指令
uchar_t g_ucHeartBeatCount = 0;
int CmdProcHeartBeat(uchar_t* pPacket){
    uchar_t frameBuf[30] = {0};
    uint_t len = FillCommPacket(frameBuf, 30, GetPackFieldCtrl(pPacket)|(1<<7), 0x05, 
                                GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket), 
                                GetPackFieldCmd(pPacket), &g_ucHeartBeatCount, 1);
    SendFrame(frameBuf, len);
    g_ucHeartBeatCount += 1;
    return 0;
}

//0x9002  查询版本指令
int CmdProcGetVersion(uchar_t* pPacket){
    uchar_t ver[2] = { THIS_UNIT_VERSION >> 8,  THIS_UNIT_VERSION & 0x00ff };
    uchar_t frameBuf[30] = {0};
    uint_t len = FillCommPacket(frameBuf, 30, GetPackFieldCtrl(pPacket)|(1<<7), 0x06, 
                                GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket), 
                                GetPackFieldCmd(pPacket), ver, 2);
    SendFrame(frameBuf, len);
    return 0;
}

// 0x9006 执行动作指令
// 68 1d 11 00 05 90 06 act(1) param(12) cs 16 -> 68 8d 08 05 00 90 06 cs 16
int CmdProcDoAction(uchar_t* pPacket){
    extern MissionInstance g_mission_inst_demo_action[];
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uint_t i = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if(paramsLen>12){
        g_mission_inst_demo_action[0].pMissionRef->pTransformerConfArr[0].pActArr[0].type = (uint_t) pParamData[0];
        for(i=0; i<6; i++){
            g_mission_inst_demo_action[0].pMissionRef->pTransformerConfArr[0].pActArr[0].args[i] 
                = (uint_t)ushort_from_big_end(pParamData[1+i*2], pParamData[2+i*2]);
        }
        g_mission_inst_demo_action[0].pMissionRef->pTransformerConfArr[0].pActArr[0].argTypeSelector = 0;
        MissionInstanceStart(pPacket, MISSION_DEMO_ACTION);
        SendPacket(0x8D, 0x04, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket), GetPackFieldCmd(pPacket), 0, 0);
    } else {
        SendErrorResponsePacket(pPacket, ERR_CMD_BAD_ARG_VAL);
    }
    return 0;
}

//0x9009  读取寄存器的值
// 68 1d 06 00 05 90 09 AA(2) zz 16 -> 68 9d 08 05 00 90 09 AA(2) VV(2)zz 16
// AA 表示寄存器地址， VV表示寄存器的值
int CmdProcGetRegValue(uchar_t* pPacket){
    uchar_t dataBuf[4] = {0};
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    ushort_t reg_addr = 0;
    ushort_t reg_val = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 1) {
        reg_addr = ((ushort_t)(pParamData[0]) << 8) + (ushort_t)(pParamData[1]);
        reg_val = FPGA(reg_addr);
    }
    dataBuf[0] = pParamData[0];
    dataBuf[1] = pParamData[1];
    dataBuf[2] = (reg_val >> 8) & 0xFF;
    dataBuf[3] = reg_val & 0xFF;
    SendPacket(0x8D, 0x08, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 4);
    return 0;
}


// 0x900A 写寄存器的值
int CmdProcWriteRegValue(uchar_t* pPacket){
    uchar_t dataBuf[4] = {0};
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    ushort_t reg_addr = 0;
    ushort_t reg_val = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 3) {
        reg_addr = ((ushort_t)(pParamData[0]) << 8) + (ushort_t)(pParamData[1]);
        reg_val = ((ushort_t)(pParamData[2]) << 8) + (ushort_t)(pParamData[3]);
        FPGA(reg_addr) = reg_val;
    }
    dataBuf[0] = pParamData[0];
    dataBuf[1] = pParamData[1];
    dataBuf[2] = (reg_val >> 8) & 0xFF;
    dataBuf[3] = reg_val & 0xFF;
    SendPacket(0x8D, 0x08, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 4);
    return 0;
}

// 0x9101 所有电机取消力矩保持
// 68 15 04 00 05 91 01 zz 16 -> 68 95 04 00 05 91 01 zz 16
int CmdProcAllMotorsHoldCancel(uchar_t* pPacket) {
    uint_t i = 0;
    for (i = 0; i < MOTOR_NUMBER; i++) {
        step_motor_hold_cancel(i);
    }
    SendPacket(0x95, 0x04, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), 0, 0);
    return 0;
}

// 0x9102 所有电机加力矩保持
// 68 15 04 00 05 91 02 zz 16 -> 68 95 04 00 05 91 02 zz 16
int CmdProcAllMotorsHoldKeep(uchar_t* pPacket) {
    uint_t i = 0;
    for (i = 0; i < MOTOR_NUMBER; i++) {
        step_motor_hold_keep(i);
    }
    SendPacket(0x95, 0x04, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), 0, 0);
    return 0;
}

// 0x9103 取消N号电机的力矩保持
// 68 1d 05 00 05 91 03 NN D7 16 -> 68 9d 04 00 05 91 03 zz 16
int CmdProcNthMotorHoldCancel(uchar_t* pPacket) {
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t motor_id = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        motor_id = pParamData[0];
        step_motor_hold_cancel(motor_id);
    }
    SendPacket(0x9D, 0x04, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), 0, 0);
    return 0;
}

// 0x9104 N号电机加力矩保持
// 68 1d 05 00 05 91 04 XX zz 16 -> 68 9d 05 00 05 91 04 XX zz 16
int CmdProcNthMotorHoldKeep(uchar_t* pPacket) {
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t motor_id = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        motor_id = pParamData[0];
        step_motor_hold_keep(motor_id);
    }
    SendPacket(0x9D, 0x05, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), &motor_id, 1);
    return 0;
}

// 0x9105 N号电机运动指令
// 68 1d 09 00 03 91 05 NN SS speed step(2) CC 16
// NN:电机号, SS:电机运动方向0-NEG_DIR,1-POS_DIR, Speed: 电机运动速度, Step:电机运动步数
int CmdProcNthMotorMove(uchar_t* pPacket) {
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t motor_id = 0;
    uchar_t motor_direction = 0;
    uchar_t motor_speed = 0;
    ushort_t motor_step = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        motor_id = pParamData[0];
        motor_direction = pParamData[1];
        motor_speed = pParamData[2];
        motor_step = ((ushort_t)(pParamData[3]) << 8) + (ushort_t)(pParamData[4]);
        step_motor_move(motor_id, motor_direction, motor_speed, motor_step);
    }
    SendPacket(0x9D, 0x04, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), 0, 0);
    return 0;
}

// 0x9106 N号电机MoveTo(pos)
// 68 1d 07 00 03 91 06 xx yy zz CD 16
// XX为电机号 yy为运动速度 zz为该电机具体位置号
int CmdProcNthMotorMoveToPos(uchar_t* pPacket){
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        uint_t common_motor_id = get_common_motor_id_from_step_motor_id(pParamData[0]);
        if (common_motor_id < COMMON_MOTORS_NUMBER) {
            ushort_t pos = pParamData[2];
            uint_t mission_type = COMMON_MOTOR_MISSION_MOVE_TO_X;
            if (0 == pos) {
                mission_type = COMMON_MOTOR_MISSION_MOVE_TO_0;
            }
            common_motor_move(pPacket, common_motor_id, mission_type, pos);
        }
        else{
            SendErrorResponsePacket(pPacket, ERR_CMD_BAD_ARG_VAL);
        }
    }
    return 0;
}

// 0x9107 N号电机初始化
// 68 1d 05 00 03 91 07 xx CD 16
// NN:电机号
int CmdProcNthMotorInit(uchar_t* pPacket){
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    int common_motor_id = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        common_motor_id = get_common_motor_id_from_step_motor_id(pParamData[0]);
        if(common_motor_id < COMMON_MOTORS_NUMBER){
            common_motor_move(pPacket, (uint_t)common_motor_id, COMMON_MOTOR_MISSION_INIT, 0);
        }
        else{
            SendErrorResponsePacket(pPacket, ERR_CMD_BAD_ARG_VAL);
        }
    }
    return 0;
}

// 0x9108 N号电机按类型运动到指定目标（位置/步数）
// 68 1d 0A 00 03 91 08 id(1) type(1) dest(4) CD 16
// 电机号(id)按类型(type)运动到目标(dest)
int CmdProcNthMotorMoveTypeToX(uchar_t* pPacket){
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 5) {
        uint_t common_motor_id = get_common_motor_id_from_step_motor_id(pParamData[0]);
        uint_t mission_type = pParamData[1];
        if (common_motor_id < COMMON_MOTORS_NUMBER && mission_type < COMMON_MOTOR_MISSION_EXTERN_TYPES_NUMBER) {
            uint_t target = ((uint_t)(pParamData[2]) << 24) | ((uint_t)(pParamData[3]) << 16) | ((uint_t)(pParamData[4]) << 8) | (uint_t)(pParamData[5]);
            common_motor_move(pPacket, common_motor_id, mission_type, target);
        }
        else{
            SendErrorResponsePacket(pPacket, ERR_CMD_BAD_ARG_VAL);
        }
    }
    return 0;
}

// 0x9201 查询所有光耦状态
// 68 0d 04 00 05 92 01 ff 16 -> 68 8d 06 00 05 92 01 sensor(2) ff 16
int CmdProcGetAllPhoteSensorStatus(uchar_t* pPacket) {
    uchar_t dataBuf[2] = {0};
    uint_t i = 0;
    extern const uint_t g_uiRegNum;  // 配置寄存器数量
    extern ushort_t g_uiRegValue[];  // 存储寄存器值的内存
    ReadAllPhotoSensorStatus();
    for (i = 0; i < 2 && i < g_uiRegNum; i++) {
        dataBuf[i] = g_uiRegValue[i];
    }
    SendPacket(0x8D, 0x06, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 2);
    return 0;
}

// 0x9202 查询某一个光耦状态
// 68 0d 05 00 05 92 02 NN ff 16 -> 68 8d 06 00 05 92 02 NN VV ff 16
// NN:表示哪个光耦，具体查询上表
// VV:表示光耦值。1-表示被挡住，0表示没挡住
int CmdProcGetPhoteSensorStatus(uchar_t* pPacket) {
    uchar_t dataBuf[2] = {0};
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t sensor_id = 0;
    uchar_t sensor_status = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        sensor_id = pParamData[0];
    }

    ReadAllPhotoSensorStatus();
    if (sensor_id < PHOTOSENSORS_NUMBER) {
        sensor_status = GetPhotoSensorStatus(sensor_id);
    }
    dataBuf[0] = sensor_id;
    dataBuf[1] = sensor_status;
    SendPacket(0x8D, 0x06, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 2);
    return 0;
}

//0x9301  查询一个阀的状态
// 68 0d 05 00 05 93 01 NN ff 16 -> 68 8d 06 00 05 93 01 NN VV ff 16
// NN:表示哪个阀
// VV:表示阀值。1-表示开，0表示关
int CmdProcGetValveStatus(uchar_t* pPacket){
    uchar_t dataBuf[2] = {0};
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t valve_id = 0;
    uchar_t valve_status = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        valve_id = pParamData[0];
    }

    if (valve_id < VALVES_NUMBER) {
        valve_status = GetValveStatus(valve_id);
    }
    dataBuf[0] = valve_id;
    dataBuf[1] = valve_status;
    SendPacket(0x8D, 0x06, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 2);
    return 0;
}

//0x9302  打开/关闭一个阀
// 68 0d 06 00 05 93 02 NN VV ff 16 -> 68 8d 06 00 05 93 02 NN VV ff 16
// NN:表示哪个阀
// VV:表示阀值。1-表示开，0表示关
int CmdProcNthValveTurn(uchar_t* pPacket){
    uchar_t dataBuf[2] = {0};
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t valve_id = 0;
    uchar_t valve_status = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 1) {
        valve_id = pParamData[0];
        valve_status = pParamData[1];
    }

    if (valve_id < VALVES_NUMBER) {
        valve_status = ValveSetOnOff(valve_id, valve_status);
    }
    dataBuf[0] = valve_id;
    dataBuf[1] = valve_status;
    SendPacket(0x8D, 0x06, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 2);
    return 0;
}

//0x9401  启动直流电机
// 68 0d 07 00 05 94 01 NN SS(2) ff 16 -> 68 8d 05 00 05 94 01 NN ff 16
// NN:表示哪个直流电机
// SS:表示速度
int CmdProcDcMotorStart(uchar_t* pPacket){
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t dc_motor_id = 0;
    ushort_t dc_motor_speed = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 2) {
        dc_motor_id = pParamData[0];
        dc_motor_speed = ((((ushort_t)pParamData[1]) << 8) & 0xFF00) + (ushort_t)(pParamData[2]);
        StartDcMotorWithSpeed(dc_motor_id, dc_motor_speed);
    }
    SendPacket(0x9D, 0x05, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), &dc_motor_id, 1);
    return 0;
}

//0x9402  停止直流电机
// 68 0d 05 00 05 94 02 NN ff 16 -> 68 8d 05 00 05 94 02 NN ff 16
// NN:表示哪个直流电机
int CmdProcDcMotorStop(uchar_t* pPacket){
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t dc_motor_id = 0;
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        dc_motor_id = pParamData[0];
        StopDcMotor(dc_motor_id);
    }
    SendPacket(0x8D, 0x05, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), &dc_motor_id, 1);
    return 0;
}

//0x9403  查询直流电机状态和配置
// 68 0d 05 00 05 94 03 NN ff 16 -> 68 8d 08 00 05 94 03 NN(1) ST(1) Speed(2) ff 16
// NN:表示哪个直流电机, ST表示工作状态，Speed 表示配置的速度
int CmdProcDcMotorGetStatus(uchar_t* pPacket){
    uchar_t dataBuf[4] = {0};
    uchar_t* pParamData = 0;
    uint_t paramsLen = 0;
    uchar_t dc_motor_id = 0;
    extern DC_MOTOR_CONFIG g_cfg_dc_motor[];   // 配置直流混匀电机基础信息
    extern DC_MOTOR_STATUS g_st_dc_motor[];    // 存储直流混匀电机状态的内存
    GetPackFieldParams(pPacket, &pParamData, &paramsLen);
    if (paramsLen > 0) {
        dc_motor_id = pParamData[0];
    }
    dataBuf[0] = dc_motor_id;
    dataBuf[1] = g_st_dc_motor[dc_motor_id].ucCurrentStatus;
    dataBuf[2] = (g_cfg_dc_motor[dc_motor_id].uiSpeedLevel >> 8) & 0xFF;
    dataBuf[3] = g_cfg_dc_motor[dc_motor_id].uiSpeedLevel & 0xFF;
    SendPacket(0x8D, 0x08, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket),
               GetPackFieldCmd(pPacket), dataBuf, 4);
    return 0;
}
