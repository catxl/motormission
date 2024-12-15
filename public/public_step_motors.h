/**********************************************************
filename   : public_step_motors.h
function   : 步进电机操作接口（公共部分）
author     : 谢令
created    : 2022-11-03
comment    :
**********************************************************/
#ifndef _PUBLIC_STEP_MOTORS_H_
#define _PUBLIC_STEP_MOTORS_H_

#include "common.h"

enum STEP_MOTOR_LOCK_STATUS{
    STEP_MOTOR_AVAILABLE,
    STEP_MOTOR_LOCKED
};

// 电机运行方向
enum MOTOR_MOVE_DIRECTION {
    MOTOR_MOVE_NEGETIVE = 0,      // 反向
    MOTOR_MOVE_POSITIVE           // 正向
};

#define FORWARD  1    //前进方向，即远离光耦方向
#define BACKWARD 0    //后退方向，即靠近光耦方向

// 普通电机配置参数
typedef struct {
    uint_t step_motor_id;     // 关联的电机ID
    uint_t move_dir[2];       // 关联电机运动方向，move_dir[0]后退方向 ，move_dir[1]前进方向
} STEP_MOTOR_CONF;

// 步进电机控制变量定义
typedef struct
{
    uint_t locked;
    ushort_t latest_direction_speed;
    int offset_steps;//记录当前步数
} MOTOR_RUNTIME_STATUS;
// 设置
void step_motor_reset_offset_steps(ushort_t motor_id);
// 设置电机方向和速度
void step_motor_set_dir_and_speed(ushort_t motor_id, ushort_t direction, ushort_t speed);
// 设置电机运动步数
void step_motor_set_step(ushort_t motor_id, ushort_t step);
// 读取电机剩余步数
ushort_t step_motor_get_remain_step(ushort_t motor_id);
// 电机运动
void step_motor_move(ushort_t motor_id, ushort_t direction, ushort_t speed, ushort_t step);
// 保持原方向运动
void step_motor_move_keep_dir(ushort_t motor_id, ushort_t speed, ushort_t step);
//按照offset步数回光耦
void step_motor_move_offset_step(ushort_t motor_id, ushort_t direction, ushort_t speed);
// 电机反向运动
void step_motor_move_reverse(ushort_t motor_id, ushort_t direction, ushort_t speed, ushort_t step);
/*
__inline void step_motor_move_reverse2(ushort_t motor_id, ushort_t direction, ushort_t speed, ushort_t step){
    step_motor_move(motor_id, 1 ^ direction, speed, step);
}
*/
// 电机停止运动
void step_motor_stop(ushort_t motor_id);
// 电机取消力矩
void step_motor_hold_cancel(ushort_t motor_id);
// 电机保持力矩
void step_motor_hold_keep(ushort_t motor_id);

/*
 * 电机占用接口
*/
// 电机是否被占用
ushort_t step_motor_is_locked(ushort_t motor_id);
// 电机是否被可用
ushort_t step_motor_is_available(ushort_t motor_id);
// 电机尝试占用，返回: 0(失败)/1(成功)
ushort_t step_motor_try_lock(ushort_t motor_id);
// 电机释放占用
void step_motor_unlock(ushort_t motor_id);
#endif
