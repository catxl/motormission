/**********************************************************
filename   : public_step_motors.c
function   : 步进电机操作接口（公共部分）
author     : 谢令
created    : 2022-11-03
comment    :
**********************************************************/
#include "public_step_motors.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
 */
extern uint_t G_MOTOR_NUMBER;  // 配置电机数量
extern ushort_t g_moter_speed_reg_addr[];
extern ushort_t g_moter_step_reg_addr[];
extern MOTOR_RUNTIME_STATUS g_moter_runtime_status[];
extern STEP_MOTOR_CONF g_cfg_step_motor[];
//=================================================================
// 电机当前运动步数清0
void step_motor_reset_offset_steps(ushort_t motor_id) {
    g_moter_runtime_status[motor_id].offset_steps = 0;
}

// 设置电机方向和速度
void step_motor_set_dir_and_speed(ushort_t motor_id, ushort_t direction, ushort_t speed) {
    g_moter_runtime_status[motor_id].latest_direction_speed = ((ushort_t)(g_cfg_step_motor[motor_id].move_dir[direction] | 0x02) << 8) + speed;
    FPGA(g_moter_speed_reg_addr[motor_id]) = g_moter_runtime_status[motor_id].latest_direction_speed;
    return;
}

// 设置电机运动步数
void step_motor_set_step(ushort_t motor_id, ushort_t step) {
    FPGA(g_moter_step_reg_addr[motor_id]) = step;
    return;
}

// 读取电机剩余步数
ushort_t step_motor_get_remain_step(ushort_t motor_id) {
    return FPGA(g_moter_step_reg_addr[motor_id]);
}

// 电机运动
void step_motor_move(ushort_t motor_id, ushort_t direction, ushort_t speed, ushort_t step) {
    //    uint_t dir = !(direction ^ g_cfg_step_motor[motor_id].forward_direction);
    ushort_t remain_step = step_motor_get_remain_step(motor_id);
    ushort_t latest_dir = (g_moter_runtime_status[motor_id].latest_direction_speed & 0x0100)>>8;  // bit8->dir

    if (remain_step > 0) {
        if (latest_dir == g_cfg_step_motor[motor_id].move_dir[FORWARD]) {
            g_moter_runtime_status[motor_id].offset_steps -= remain_step;
        } else {
            g_moter_runtime_status[motor_id].offset_steps += remain_step;
        }
    }
    if (direction == FORWARD) {
        g_moter_runtime_status[motor_id].offset_steps += step;
    } else {
        g_moter_runtime_status[motor_id].offset_steps -= step;
    }
    step_motor_set_dir_and_speed(motor_id, direction, speed);
    step_motor_set_step(motor_id, step);
    return;
}

// 保持原方向运动
void step_motor_move_keep_dir(ushort_t motor_id, ushort_t speed, ushort_t step) {
    ushort_t latest_dir = (g_moter_runtime_status[motor_id].latest_direction_speed & 0x0100)>>8;  // bit8->dir
    if (latest_dir == g_cfg_step_motor[motor_id].move_dir[FORWARD]) {
        latest_dir = FORWARD;
    } else {
        latest_dir = BACKWARD;
    }
    step_motor_move(motor_id, latest_dir, speed, step);
}

// 电机按照offsetstep步数回光耦
void step_motor_move_offset_step(ushort_t motor_id, ushort_t direction, ushort_t speed) {
    uint_t offsetstep = 0;
    if(g_moter_runtime_status[motor_id].offset_steps>0){
        offsetstep = g_moter_runtime_status[motor_id].offset_steps;
    }
    else{
        offsetstep = 0 - g_moter_runtime_status[motor_id].offset_steps;
    }
    step_motor_move(motor_id, direction, speed, offsetstep);
}

// 电机反向运动
void step_motor_move_reverse(ushort_t motor_id, ushort_t direction, ushort_t speed, ushort_t step) {
    step_motor_move(motor_id, 1 ^ direction, speed, step);
}

// 电机停止运动
void step_motor_stop(ushort_t motor_id) {
    step_motor_set_step(motor_id, 0);
    return;
}

// 电机取消力矩
void step_motor_hold_cancel(ushort_t motor_id) {
    g_moter_runtime_status[motor_id].latest_direction_speed = g_moter_runtime_status[motor_id].latest_direction_speed & 0xFDFF;
    FPGA(g_moter_speed_reg_addr[motor_id]) = g_moter_runtime_status[motor_id].latest_direction_speed;
    step_motor_set_step(motor_id, 0);
    return;
}

// 电机保持力矩
void step_motor_hold_keep(ushort_t motor_id) {
    g_moter_runtime_status[motor_id].latest_direction_speed = g_moter_runtime_status[motor_id].latest_direction_speed | 0x0200;
    FPGA(g_moter_speed_reg_addr[motor_id]) = g_moter_runtime_status[motor_id].latest_direction_speed;
    step_motor_set_step(motor_id, 0);
    return;
}

// 电机是否被占用
ushort_t step_motor_is_locked(ushort_t motor_id) {
    if (STEP_MOTOR_LOCKED == g_moter_runtime_status[motor_id].locked) {
        return 1;
    }
    return 0;
}

// 电机是否被可用
ushort_t step_motor_is_available(ushort_t motor_id) {
    if (STEP_MOTOR_AVAILABLE == g_moter_runtime_status[motor_id].locked) {
        return 1;
    }
    return 0;
}

// 电机尝试占用，返回: 0(失败)/1(成功)
ushort_t step_motor_try_lock(ushort_t motor_id) {
    if (STEP_MOTOR_AVAILABLE == g_moter_runtime_status[motor_id].locked) {
        g_moter_runtime_status[motor_id].locked = STEP_MOTOR_LOCKED;
        return 1;
    }
    return 0;
}

// 电机释放占用
void step_motor_unlock(ushort_t motor_id) {
    g_moter_runtime_status[motor_id].locked = STEP_MOTOR_AVAILABLE;
}
