/**********************************************************
filename   : cmd_proc_share_func.h
function   : 命令处理函数(通用部分)
author     : 谢令
created    : 2022-11-21
comment    :
**********************************************************/
#ifndef _CMD_PROC_SHARE_FUNC_H_
#define _CMD_PROC_SHARE_FUNC_H_
#include "common.h"

// 0x9001 心跳指令
int CmdProcHeartBeat(uchar_t* pPacket);
// 0x9002 查询版本指令
int CmdProcGetVersion(uchar_t* pPacket);
// 0x9006 查询版本指令
int CmdProcDoAction(uchar_t* pPacket);
// 0x9009 读取寄存器的值
int CmdProcGetRegValue(uchar_t* pPacket);
// 0x900A 写寄存器的值
int CmdProcWriteRegValue(uchar_t* pPacket);
// 0x9101 所有电机取消力矩保持
int CmdProcAllMotorsHoldCancel(uchar_t* pPacket);
// 0x9102 所有电机加力矩保持
int CmdProcAllMotorsHoldKeep(uchar_t* pPacket);
// 0x9103 取消N号电机的力矩保持
int CmdProcNthMotorHoldCancel(uchar_t* pPacket);
// 0x9104 N号电机加力矩保持
int CmdProcNthMotorHoldKeep(uchar_t* pPacket);
// 0x9105 N号电机运动指令
int CmdProcNthMotorMove(uchar_t* pPacket);
// 0x9106 N号电机MoveTo(pos)
int CmdProcNthMotorMoveToPos(uchar_t* pPacket);
// 0x9107 N号电机初始化
int CmdProcNthMotorInit(uchar_t* pPacket);
// 0x9108 N号电机按类型运动到指定目标（位置/步数）
int CmdProcNthMotorMoveTypeToX(uchar_t* pPacket);
// 0x9201 查询所有光耦状态
int CmdProcGetAllPhoteSensorStatus(uchar_t* pPacket);
// 0x9202 查询当前一个光耦值
int CmdProcGetPhoteSensorStatus(uchar_t* pPacket);
// 0x9301 查询一个阀的状态
int CmdProcGetValveStatus(uchar_t* pPacket);
// 0x9302 打开/关闭一个阀
int CmdProcNthValveTurn(uchar_t* pPacket);
// 0x9401 启动直流电机
int CmdProcDcMotorStart(uchar_t* pPacket);
// 0x9402 停止直流电机
int CmdProcDcMotorStop(uchar_t* pPacket);
// 0x9403 查询直流电机状态和配置
int CmdProcDcMotorGetStatus(uchar_t* pPacket);

#endif
