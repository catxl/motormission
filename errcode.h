/**********************************************************
filename   : errcode.h
function   : 错误码
author     : 谢令
created    : 2022-7-1
comment    :
**********************************************************/
#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

enum {
    RET_OK = 0,           // 正确返回
    ERR_CMD_ID,           // 命令标识错误
    ERR_CMD_FUNCTION,     // 控制码中的功能码错误
    ERR_CMD_DATA_LENGTH,  // 数据长度错误
    ERR_CMD_CHECK,        // 校验码错误
    ERR_CMD_ARGS,         // 命令标识后的参数错误
    ERR_CMD_END,          // 结束码故障
    ERR_CMD_BAD_ARG_VAL,  // 参数值异常
    // MISSION异常
    ERR_MISSION_CONFLICT, // 任务冲突
    // 电机运动异常
    ERR_MOTOR_STEPS_OVERRUN,    // 电机步数超限
    ERR_MOTOR_NOT_MEET_SENSOR,  // 电机移动未遇见光耦
    ERR_MOTOR_NOT_REACHED,      // 电机运行未触达目标
    ERR_MOTOR_UNREACHABLE,      // 电机运行目标不可触达
    // 错误
    ERR_UNKNOWN,  // 未知错误
};

#endif
