/**********************************************************
filename   : protocol.c
function   : 协议定义
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#include "protocol.h"
#include "cmd_proc_share_func.h"
#include "cmd_proc_func.h"


COMM_CMD_CFG g_cfg_comm_cmd[COMM_CMD_NUMBER] = {
//  cmdId,                                          cmdCode,             isFixedLen,         paramsLenth,       procFunc
//  序号                                             编码                 是否固定长度         命令参数的长度      命令处理函数
    {COMM_CMD_DO_DEMO,                              0x0001,              1,                  0,                 CmdProcExecDemo}, 
// share ...
    {COMM_CMD_DBG_HEARTBEAT,                        0x9001,              1,                  0,                 CmdProcHeartBeat},               // 心跳指令
    {COMM_CMD_DBG_GET_VERSION,                      0x9002,              1,                  0,                 CmdProcGetVersion},              // 查询版本指令
    {COMM_CMD_DBG_DO_ACTION,                        0x9006,              1,                 13,                 CmdProcDoAction},                // 执行指定动作
    {COMM_CMD_DBG_GET_REG_VALUE,                    0x9009,              1,                  2,                 CmdProcGetRegValue},             // 查询某一个寄存器的值
    {COMM_CMD_DBG_WRITE_REG_VALUE,                  0x900A,              1,                  2,                 CmdProcWriteRegValue},           // 写入某一个寄存器的值
    {COMM_CMD_DBG_ALL_MOTORS_HOLD_CANCEL,           0x9101,              1,                  0,                 CmdProcAllMotorsHoldCancel},     // 所有电机取消力矩保持
    {COMM_CMD_DBG_ALL_MOTORS_HOLD_KEEP,             0x9102,              1,                  0,                 CmdProcAllMotorsHoldKeep},       // 所有电机加力矩保持
    {COMM_CMD_DBG_NTH_MOTOR_HOLD_CANCEL,            0x9103,              1,                  1,                 CmdProcNthMotorHoldCancel},      // 取消N号电机的力矩保持
    {COMM_CMD_DBG_NTH_MOTOR_HOLD_KEEP,              0x9104,              1,                  1,                 CmdProcNthMotorHoldKeep},        // N号电机加力矩保持
    {COMM_CMD_DBG_NTH_MOTOR_MOVE,                   0x9105,              1,                  5,                 CmdProcNthMotorMove},            // N号电机运动指令
    {COMM_CMD_DBG_NTH_COMMON_MOTOR_MOVE_TO_POS,     0x9106,              1,                  3,                 CmdProcNthMotorMoveToPos},       // N号普通电机运动到指定位置
    {COMM_CMD_DBG_NTH_COMMON_MOTOR_INIT,            0x9107,              1,                  1,                 CmdProcNthMotorInit},            // N号普通电机初始化
    {COMM_CMD_DBG_NTH_COMMON_MOTOR_MOVE_TYPE_TO_X,  0x9108,              1,                  6,                 CmdProcNthMotorMoveTypeToX},     // N号普通电机按类型运动到指定位置}
    {COMM_CMD_DBG_GET_ALL_SENSOR,                   0x9201,              1,                  0,                 CmdProcGetAllPhoteSensorStatus}, // 查询所有光耦状态
    {COMM_CMD_DBG_GET_X_SENSOR,                     0x9202,              1,                  1,                 CmdProcGetPhoteSensorStatus},    // 查询某一个光耦状态
    {COMM_CMD_DBG_GET_X_VALVE,                      0x9301,              1,                  1,                 CmdProcGetValveStatus},          // 查询某一个阀状态
    {COMM_CMD_DBG_SET_X_VALVE,                      0x9302,              1,                  2,                 CmdProcNthValveTurn},            // 设置(打开/关闭)某一个阀
    {COMM_CMD_DBG_DC_MOTOR_START,                   0x9401,              1,                  3,                 CmdProcDcMotorStart},            // 启动直流电机
    {COMM_CMD_DBG_DC_MOTOR_STOP,                    0x9402,              1,                  1,                 CmdProcDcMotorStop},             // 停止直流电机
    {COMM_CMD_DBG_DC_MOTOR_GET_STATUS,              0x9403,              1,                  1,                 CmdProcDcMotorGetStatus},        // 查询直流电机工作状态和参数
    {COMM_CMD_UNKNOWN,                              0x0000,              0,                  0,                 0},
};
