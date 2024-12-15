/**********************************************************
filename   : public_common_motors_extern.h
function   : 普通电机操作接口（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#ifndef _PUBLIC_COMMON_MOTORS_EXTERN_H_
#define _PUBLIC_COMMON_MOTORS_EXTERN_H_

#include "common.h"
#include "public_common_motors.h"

enum COMMON_MOTOR_MISSION_EXTERN_TYPE{
    COMMON_MOTOR_MISSION_EXTERN_TYPE_BEGIN = COMMON_MOTOR_MISSION_TYPES_NUMBER - 1,  // <--!!这行不能删
    // TODO: 添加扩展类型
    COMMON_MOTOR_MISSION_EXTERN_TYPES_NUMBER
};

enum MOVE_STRATEGY_EXTERN_TYPE{
    MOVE_STRATEGY_EXTERN_TYPE_BEGIN = MOVE_STRATEGY_TYPE_NUM - 1,  // <--!!这行不能删
    // TODO: 添加扩展类型
    MOVE_STRATEGY_EXTERN_TYPES_NUMBER
};

enum MOTOR_STOPPED_CHECKING_EXTERN_TYPE {
    MOTOR_STOPPED_CHECKING_EXTERN_TYPE_BEGIN = MOTOR_STOPPED_CHECKING_TYPE_NUM - 1,  // <--!!这行不能删
    // TODO: 添加扩展类型
    MOTOR_STOPPED_CHECKING_EXTERN_TYPE_NUM
};

// 按策略运动
uint_t common_motor_extern_move_by_strategy(uint_t motor_id, uint_t stargety_id, uint_t amount);

// 计算普通电机探测器参数
uint_t common_motor_extern_calc_detector_param(uint_t common_motor_id, uint_t mission_type, uint_t mission_arg1);

ushort_t common_motor_extern_is_stopped(uint_t common_motor_id);

#endif
