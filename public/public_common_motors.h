/**********************************************************
filename   : public_common_motors.h
function   : 普通电机接口定义（公共部分）
author     : 谢令
created    : 2022-12-05
comment    :
**********************************************************/
#ifndef _PUBLIC_COMMON_MOTORS_H_
#define _PUBLIC_COMMON_MOTORS_H_

#include "common.h"
#include "public_step_motors.h"

#define TRANS_TO_TARGET(dir, speed, destination)  (((dir&0xFF)<<24)|((speed&0xFF)<<16)|(destination&0xFFFF))
#define GET_DIR(target) ((target>>24)&0xFF)
#define GET_SPEED(target) ((target>>16)&0xFF)
#define GET_DEST(target) ((target)&0xFFFF)

enum MOVE_STRATEGY_TYPE {
    MOVE_STG_DO_NOTHIN,                   // 不动
    MOVE_STG_DSX_STEPS,                   // DSX: Dir(配置值)|Speed(配置值)|Dest(计算值)，Dest = STEPS
    MOVE_STG_XSX_STEPS,                   // XSX: Dir(计算值)|Speed(配置值)|Dest(计算值)，Dest = STEPS
    MOVE_STG_DSD_STEPS,                   // DSD: Dir(配置值)|Speed(配置值)|Dest(配置值)，Dest = STEPS
    MOVE_STG_XSD_KEEP_DIR,                // XSD: Dir(计算值)|Speed(配置值)|Dest(配置值)，保持原方向
    MOVE_STG_DSX_POS,                     // DSX: Dir(配置值)|Speed(配置值)|Dest(计算值)，Dest = POS
    MOVE_STG_DSX_CIRCLE_POS,              // DSX: Dir(配置值)|Speed(配置值)|Dest(计算值)，Dest = CIRCLE_POS
    MOVE_STG_DSX_CIRCLE_ISOMETRY_POS,     // DSX: Dir(配置值)|Speed(配置值)|Dest(计算值)，Dest = CIRCLE_ISOMETRY_POS（圆形等距孔位）
    MOVE_STG_DSX_OFFSET_STEP,             // DSX: Dir(配置值)|Speed(配置值)|Dest(计算值)，Dest = OFFSET_STEP
    MOVE_STG_DXX_STEPS,                   // DSD: Dir(配置值)|Speed(计算值)|Dest(计算值)，Dest = STEPS，Speed根据步数计算
    MOVE_STG_XXX_STEPS,                   // DSD: Dir(计算值)|Speed(计算值)|Dest(计算值)，Dest = STEPS，使用输入参数
    MOVE_STG_XXX_POS_SPEED,               // DSD: Dir(计算值)|Speed(计算值)|Dest(计算值)，Dest = POS，Step&Speed都根据pos查询得到
    MOVE_STRATEGY_TYPE_NUM
};

enum MOTOR_STOPPED_CHECKING_TYPE {
    MOTOR_STOPPED_CHECK_BY_NORMAL,
    MOTOR_STOPPED_CHECKING_TYPE_NUM
};

// 动作执行策略结构
typedef struct {
    uint_t    id;               // 编号，用来确认配置顺序
    uint_t    type;
    uchar_t   direction;
    uchar_t   speed;
    ushort_t  step;
    uint_t*   pRef;
    uint_t*   pSpeedRef;
} COMMON_MOTOR_ACT_STRATEGY_CONF;


enum COMMON_MOTOR_MISSION_TYPE{
    COMMON_MOTOR_MISSION_INIT,
    COMMON_MOTOR_MISSION_MOVE_TO_0,
    COMMON_MOTOR_MISSION_MOVE_TO_X,
    COMMON_MOTOR_MISSION_TYPES_NUMBER
};

enum COMMON_MOTOR_COMPENSATE_STAGE_ENUM {
    COMMON_MOTOR_COMPENSATE_READY,
    COMMON_MOTOR_COMPENSATE_START,
    COMMON_MOTOR_COMPENSATE_WAIT_STOP_1,
    COMMON_MOTOR_COMPENSATE_WAIT_STOP_2,
    COMMON_MOTOR_COMPENSATE_WAIT_FORCE_STOP,
    COMMON_MOTOR_COMPENSATE_DONE,
};

// 运行状态定义
typedef struct
{
    uint_t current_mission_id;
    uint_t current_position;
    uint_t cmd_target;
    uint_t mission_type;
    uint_t compensate_stage;
    uint_t compensate_steps;
} COMMON_MOTOR_RUNTIME_STATUS;

// 普通电机配置参数
typedef struct {
    uint_t common_motor_id;         // 普通电机ID，用来确认配置顺序
    uint_t step_motor_id;           // 关联的电机ID
    uint_t max_pos_no;              // 最大位置编号
    uint_t stopped_check_type;      // "电机停止"的检测方式 
    uint_t stopped_check_param;     // "电机停止"的检测参数 
} COMMON_MOTOR_CONF;

// 定义任务动作完成后的执行函数指针类型
typedef void (*COMMON_MOTOR_MISSION_DONE_FUNC)(uint_t common_motor_id, uint_t mission_type, uint_t error_code);

// 普通电机任务参数配置
typedef struct {
    uint_t common_motor_act_id;                    // 普通电机动作ID，用来确认配置顺序
    uint_t mission_id;                             // 关联的移动任务ID
    COMMON_MOTOR_MISSION_DONE_FUNC p_done_func;    // 任务完成函数
    uint_t dect_id;                                // 探测器ID
    uint_t errcode;                                // 探测器未触发的返回码
    uint_t* p_act_stg_step_arr;                    // 动作步骤策略配置指针
    uint_t compensate_steps;                       // 补偿步数
    uint_t timer_id;                               // 定时器ID
} COMMON_MOTOR_MISSION_CONF;

// 执行配置的动作
uint_t common_motor_move_step_x(uint_t common_motor_id, uint_t mission_type, uint_t step_id);
void common_motor_start_compensate(uint_t common_motor_id, uint_t mission_type);
void common_motor_compensate_tick(uint_t common_motor_id);
uint_t common_motor_is_compensate_done(uint_t common_motor_id);

/*
* 普通电机运动超级入口
* 当 mission_type == COMMON_MOTOR_MISSION_INIT 时,              执行初始化动作
* 当 mission_type == COMMON_MOTOR_MISSION_MOVE_TO_X 时,         执行“移动到 to_pos 位置”的动作
* 当 mission_type == COMMON_MOTOR_MISSION_MOVE_TO_0 时,         执行“回到 0 位”的动作
* 当 mission_type == COMMON_MOTOR_MISSION_MOVE_INTO_LIQUID 时,  执行对应电机扎入液位的动作
*/
uint_t common_motor_move(uchar_t* pPacket, uint_t common_motor_id, uint_t mission_type, uint_t target);
uint_t common_motor_move3(uchar_t* pPacket, uint_t common_motor_id, uint_t mission_type, uint_t dest, uint_t speed, uint_t dir);

// 电机动作结束
uint_t common_motor_move_finished(uint_t common_motor_id, uint_t mission_type, uint_t error_code);

// 电机动作是否完成
uint_t common_motor_is_move_done(uint_t common_motor_id);
// 电机动作是否停止
uint_t common_motor_is_move_succ(uint_t common_motor_id);
// 电机动作是否停止
uint_t common_motor_is_move_fail(uint_t common_motor_id);

/*
 * 普通电机加锁解锁接口
*/
uint_t common_motor_try_lock(uint_t common_motor_id);
void common_motor_unlock(uint_t common_motor_id);
// 获取普通电机剩余步数
ushort_t common_motor_get_remain_step(uint_t common_motor_id);
ushort_t common_motor_is_stopped(uint_t common_motor_id);
// 获取当前'位置'
uint_t common_motor_get_current_pos(uint_t common_motor_id);

/*
 * 按策略运动
*/
uint_t common_motor_move_by_strategy(uint_t motor_id, uint_t stargety_id, uint_t amount);

int get_common_motor_id_from_step_motor_id(uint_t step_motor_id);

#endif
