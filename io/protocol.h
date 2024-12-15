/**********************************************************
filename   : protocol.h
function   : 协议定义
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#ifndef _IO_PROTOCOL_H_
#define _IO_PROTOCOL_H_

#include "common.h"

enum COMM_CMD_IDS{
    COMM_CMD_DO_DEMO,                                // 0x0001 执行Demo
// share ...
    COMM_CMD_DBG_HEARTBEAT,                          // 0x9001 心跳指令
    COMM_CMD_DBG_GET_VERSION,                        // 0x9002 查询版本指令
    COMM_CMD_DBG_DO_ACTION,                          // 0x9006 执行指定动作
    COMM_CMD_DBG_GET_REG_VALUE,                      // 0x9009 查询某一个寄存器的值
    COMM_CMD_DBG_WRITE_REG_VALUE,                    // 0x900A 写入某一个寄存器的值
    COMM_CMD_DBG_ALL_MOTORS_HOLD_CANCEL,             // 0x9101 所有电机取消力矩保持
    COMM_CMD_DBG_ALL_MOTORS_HOLD_KEEP,               // 0x9102 所有电机加力矩保持
    COMM_CMD_DBG_NTH_MOTOR_HOLD_CANCEL,              // 0x9103 取消N号电机的力矩保持
    COMM_CMD_DBG_NTH_MOTOR_HOLD_KEEP,                // 0x9104 N号电机加力矩保持
    COMM_CMD_DBG_NTH_MOTOR_MOVE,                     // 0x9105 N号电机运动指令
    COMM_CMD_DBG_NTH_COMMON_MOTOR_MOVE_TO_POS,       // 0x9106 N号普通电机运动到指定位置
    COMM_CMD_DBG_NTH_COMMON_MOTOR_INIT,              // 0x9107 N号普通电机初始化
    COMM_CMD_DBG_NTH_COMMON_MOTOR_MOVE_TYPE_TO_X,    // 0x9108 N号普通电机按类型运动到指定位置
    COMM_CMD_DBG_GET_ALL_SENSOR,                     // 0x9201 查询所有光耦状态
    COMM_CMD_DBG_GET_X_SENSOR,                       // 0x9202 查询某一个光耦状态
    COMM_CMD_DBG_GET_X_VALVE,                        // 0x9301 查询某一个阀状态
    COMM_CMD_DBG_SET_X_VALVE,                        // 0x9302 设置(打开/关闭)某一个阀
    COMM_CMD_DBG_DC_MOTOR_START,                     // 0x9401 启动直流电机
    COMM_CMD_DBG_DC_MOTOR_STOP,                      // 0x9402 停止直流电机
    COMM_CMD_DBG_DC_MOTOR_GET_STATUS,                // 0x9403 查询直流电机工作状态和参数
    COMM_CMD_UNKNOWN,
    COMM_CMD_NUMBER
};

// 定义指令处理函数指针类型
typedef int (*CMD_PROC_FUNC)(uchar_t* pPacket);

typedef struct {
    uint_t    cmdId;            // 命令序号
    ushort_t  cmdCode;          // 命令编码
    uint_t    isFixedLen;       // 是否固定长度
    uint_t    paramsLenth;      // 命令参数的长度
    CMD_PROC_FUNC  procFunc;    // 命令处理函数
} COMM_CMD_CFG;

extern COMM_CMD_CFG g_cfg_comm_cmd[];


#endif
