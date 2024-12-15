/**********************************************************
filename   : public_common_motors.c
function   : 普通电机接口定义（公共部分）
author     : 谢令
created    : 2022-12-05
comment    :
**********************************************************/
#include "public_common_motors_extern.h"
#include "public_missions.h"
#include "errcode.h"
#include "public_detectors_extern.h"
#include "public_utils.h"
#include "utils.h"


// 底层步进电机的寄存器配置
extern DETECTOR_CONFIG g_cfg_detector[];          // 配置探测器基础信息

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern uint_t G_COMMON_MOTORS_NUMBER;                                             // 配置普通电机数量
extern COMMON_MOTOR_CONF g_cfg_common_motors[];                                   // 配置普通电机通用属性
extern COMMON_MOTOR_ACT_STRATEGY_CONF g_cfg_common_motor_sub_actions_strategy[];  // 普通电机细分（步骤）动作配置
extern COMMON_MOTOR_MISSION_CONF      g_cfg_common_motor_mission[];               // 普通电机动作的任务参数配置
extern uint_t                         g_cfg_common_motor_mission_map[];           // 普通电机的 【普通电机ID + 任务类型】 --> 任务参数配置  映射表
extern COMMON_MOTOR_RUNTIME_STATUS    g_common_motor_status[];                    // 运行状态记录
//=================================================================

#define MISSION_TYPES_NUM (COMMON_MOTOR_MISSION_EXTERN_TYPES_NUMBER)

int get_common_motor_id_from_step_motor_id(uint_t step_motor_id){
    int i = 0;
    for(i=0; i<G_COMMON_MOTORS_NUMBER; i++){
        if(g_cfg_common_motors[i].step_motor_id == step_motor_id){
            return i;
        }
    }
    return -1;
}

// 按照不同策略控制电机运动
uint_t common_motor_move_by_strategy(uint_t common_motor_id, uint_t stargety_id, uint_t target){
    ushort_t motor_id = g_cfg_common_motors[common_motor_id].step_motor_id;
    uint_t direction = g_cfg_common_motor_sub_actions_strategy[stargety_id].direction;
    ushort_t speed = g_cfg_common_motor_sub_actions_strategy[stargety_id].speed;
    uint_t steps = g_cfg_common_motor_sub_actions_strategy[stargety_id].step;
    switch(g_cfg_common_motor_sub_actions_strategy[stargety_id].type){
        case MOVE_STG_DO_NOTHIN:{
            break;
        }
        case MOVE_STG_DSX_STEPS:{           // 按步数运行到指定位置：方向(配置)、速度(配置)、步数(输入)
            steps = GET_DEST(target);
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_XSX_STEPS:{           
            if (GET_DIR(target)){
                direction = FORWARD;
            } else {
                direction = BACKWARD;
            }
            step_motor_move(motor_id, direction, speed, GET_DEST(target) & 0x7fff);
            break;
        }
        case MOVE_STG_DSD_STEPS:{           // 按步数运行到指定位置：方向(配置)、速度(配置)、步数(配置)
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_XSD_KEEP_DIR:{        // 按步数运行到指定位置：方向(保持原方向)、速度(配置)、步数(配置)
            step_motor_move_keep_dir(motor_id, speed, steps);
            break;
        }
        case MOVE_STG_DSX_POS:{             // 按位置编号运行到指定位置：方向(计算)、速度(配置)、步数(计算)
            uint_t dest = GET_DEST(target);
            uint_t src_step = g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef[g_common_motor_status[common_motor_id].current_position];
            uint_t dst_step = g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef[dest];
            direction = FORWARD;
            if (dst_step < src_step){
                direction = BACKWARD;
                steps = src_step - dst_step;
            }
            else{
                steps = dst_step - src_step;
            }
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_DSX_CIRCLE_POS:{           // 按环形位置编号运行到指定位置：方向(计算)、速度(配置)、步数(计算)
            uint_t circle_poionts_num = g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef[0];  // 总的孔位数
            uint_t* pos_steps = &(g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef[1]);       // 固定位置的步数
            uint_t src_pos = g_common_motor_status[common_motor_id].current_position;                  // 当前位置编号
            uint_t dest_pos = GET_DEST(target);                                                        // 目标位
            get_circle_move_dir_steps(src_pos, dest_pos, circle_poionts_num, pos_steps, &direction, &steps);
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_DSX_CIRCLE_ISOMETRY_POS:{  // 圆形等距孔位
            uint_t circle_poionts_num = g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef[0];  // 总的孔位数
            uint_t circle_steps = g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef[1];        // 整圈总步数
            uint_t src_pos = g_common_motor_status[common_motor_id].current_position;                  // 当前位置编号
            uint_t dest_pos = GET_DEST(target);                                                         // 目标位
            uint_t pos_num = 0;
            get_circle_isometry_pos_move_dir_steps(src_pos, dest_pos, circle_poionts_num, circle_steps, &direction, &steps, &pos_num);
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_DSX_OFFSET_STEP: {         // 按偏移步数运行到光耦位：方向(配置)、速度(配置)
            step_motor_move_offset_step(motor_id, direction, speed);
            break;
        }
        case MOVE_STG_DXX_STEPS: {               // DSD: Dir(配置值)|Speed(计算值)|Dest(计算值)，Dest = STEPS，Speed根据步数计算
            steps = GET_DEST(target);
            speed = get_speed_by_steps((ushort_t)steps, g_cfg_common_motor_sub_actions_strategy[stargety_id].pSpeedRef);
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_XXX_STEPS: {               // DSD: Dir(计算值)|Speed(计算值)|Dest(计算值)，Dest = STEPS，使用输入参数
            direction = GET_DIR(target);
            speed = GET_SPEED(target);
            steps = GET_DEST(target);
            step_motor_move(motor_id, direction, speed, steps);
            break;
        }
        case MOVE_STG_XXX_POS_SPEED: {          // DSD: Dir(计算值)|Speed(计算值)|Dest(计算值)，Dest = POS，Step&Speed都根据pos查询得到
            ushort_t src_pos = g_common_motor_status[common_motor_id].current_position;                  // 当前位置编号
            ushort_t dest_pos = GET_DEST(target);                                                        // 目标位
            if (src_pos != dest_pos) {
                ushort_t dst_dir = GET_DIR(target);
                ushort_t dst_speed = GET_SPEED(target);
                ushort_t dst_speps = 0;
                get_steps_speed_by_pos(src_pos, dest_pos, g_cfg_common_motor_sub_actions_strategy[stargety_id].pRef, &dst_dir, &dst_speed, &dst_speps);
                step_motor_move(motor_id, dst_dir, dst_speed, dst_speps);
            }
            break;
        }
        default: {
            return common_motor_extern_move_by_strategy(common_motor_id, stargety_id, target);
        }
    }
    return 0;
}

// 执行配置的动作
uint_t common_motor_move_step_x(uint_t common_motor_id, uint_t mission_type, uint_t step_id){
    uint_t miss_idx = g_cfg_common_motor_mission_map[MISSION_TYPES_NUM*common_motor_id + mission_type];
    return common_motor_move_by_strategy(common_motor_id, 
                                         g_cfg_common_motor_mission[miss_idx].p_act_stg_step_arr[step_id],
                                         g_common_motor_status[common_motor_id].cmd_target);
}

void common_motor_start_compensate(uint_t common_motor_id, uint_t mission_type){
    uint_t miss_idx = g_cfg_common_motor_mission_map[MISSION_TYPES_NUM*common_motor_id + mission_type];
    g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_START;
    g_common_motor_status[common_motor_id].compensate_steps = g_cfg_common_motor_mission[miss_idx].compensate_steps;
    if(0 == g_cfg_common_motor_mission[miss_idx].compensate_steps){
        g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_DONE;
    }
}

void common_motor_compensate_tick(uint_t common_motor_id){
    switch(g_common_motor_status[common_motor_id].compensate_stage){
        case COMMON_MOTOR_COMPENSATE_READY:{
            break;
        }
        case COMMON_MOTOR_COMPENSATE_START:{
            ushort_t remain_steps = common_motor_get_remain_step(common_motor_id);
            uint_t compensate_steps = g_common_motor_status[common_motor_id].compensate_steps;
            if (remain_steps == compensate_steps) {
                g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_WAIT_STOP_2;
            } else if (remain_steps < compensate_steps) {
                g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_WAIT_STOP_1;
                g_common_motor_status[common_motor_id].compensate_steps = compensate_steps - remain_steps;
            } else if (remain_steps > compensate_steps) {
                g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_WAIT_FORCE_STOP;
                g_common_motor_status[common_motor_id].compensate_steps = remain_steps - compensate_steps;
            }
            break;
        }
        case COMMON_MOTOR_COMPENSATE_WAIT_STOP_1:{
            if(common_motor_is_stopped(common_motor_id)){
                step_motor_move_keep_dir(g_cfg_common_motors[common_motor_id].step_motor_id, 0, g_common_motor_status[common_motor_id].compensate_steps);
                g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_WAIT_STOP_2;
            }
            break;
        }
        case COMMON_MOTOR_COMPENSATE_WAIT_STOP_2:{
            if(common_motor_is_stopped(common_motor_id)){
                g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_DONE;
            }
            break;
        }
        case COMMON_MOTOR_COMPENSATE_WAIT_FORCE_STOP:{
            ushort_t remain_steps = common_motor_get_remain_step(common_motor_id);
            if(remain_steps <= g_common_motor_status[common_motor_id].compensate_steps){
                step_motor_stop(g_cfg_common_motors[common_motor_id].step_motor_id);
                g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_DONE;
            }
        }
        case COMMON_MOTOR_COMPENSATE_DONE:{
            break;
        }
        default:{
            break;
        }
    }
}

uint_t common_motor_is_compensate_done(uint_t common_motor_id){
    common_motor_compensate_tick(common_motor_id);
    if(COMMON_MOTOR_COMPENSATE_DONE == g_common_motor_status[common_motor_id].compensate_stage){
        g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_READY;
        return 1;
    }
    return 0;
}

// 计算普通电机探测器参数
uint_t common_motor_calc_detector_param(uint_t common_motor_id, uint_t mission_type, uint_t mission_arg1){
    uint_t miss_idx = g_cfg_common_motor_mission_map[MISSION_TYPES_NUM*common_motor_id + mission_type];
    // 根据探测器类型生成探测器启动参数
    switch (g_cfg_detector[g_cfg_common_motor_mission[miss_idx].dect_id].dectType) {
        case DECT_TYPE_PASS:
        case DECT_TYPE_REJECT:
        case DECT_TYPE_PHOTOSENSOR:
        case DECT_TYPE_REVERSE_PHOTOSENSOR:
        case DECT_TYPE_TIMEOUT:{
            return mission_arg1;
        }
        default:{
            break;
        }
    }
    return common_motor_extern_calc_detector_param(common_motor_id, mission_type, mission_arg1);
}

/*
* 普通电机运动超级入口
* 当 mission_type == COMMON_MOTOR_MISSION_INIT 时,              执行初始化动作
* 当 mission_type == COMMON_MOTOR_MISSION_MOVE_TO_X 时,         执行“移动到 target 位置”的动作
* 当 mission_type == COMMON_MOTOR_MISSION_MOVE_TO_0 时,         执行“回到 0 位”的动作
* 当 mission_type == COMMON_MOTOR_MISSION_MOVE_INTO_LIQUID 时,  执行对应电机扎入液位的动作
* 参数：target(32bit) = H--> [direction(8bit)|speed(8bit)|pos(16bit)] <--L
*/
uint_t common_motor_move(uchar_t* pPacket, uint_t common_motor_id, uint_t mission_type, uint_t target){
    uint_t miss_idx = g_cfg_common_motor_mission_map[MISSION_TYPES_NUM*common_motor_id + mission_type];
    uint_t mission_id = g_cfg_common_motor_mission[miss_idx].mission_id;
    uint_t dect_id = g_cfg_common_motor_mission[miss_idx].dect_id;
    uint_t dect_param1 = 0;
    uint_t errcode = g_cfg_common_motor_mission[miss_idx].errcode;
    uint_t timer_id = g_cfg_common_motor_mission[miss_idx].timer_id;
    uint_t dest = 0;
    //uint_t speed = (target>>16)&0xFF;
    if(MissionInstanceIsWorking(mission_id)){
        if(pPacket){
            SendErrorResponsePacket(pPacket, ERR_MISSION_CONFLICT);
        }
        return 0xFFFFFFFF;
    }
    if(COMMON_MOTOR_MISSION_MOVE_TO_0 == mission_type || COMMON_MOTOR_MISSION_INIT == mission_type){
        target = 0;
    }
    dest = GET_DEST(target);
    dect_param1 = common_motor_calc_detector_param(common_motor_id, mission_type, target);
    g_common_motor_status[common_motor_id].cmd_target = target;
    g_common_motor_status[common_motor_id].current_mission_id = mission_id;
    g_common_motor_status[common_motor_id].mission_type = mission_type;
    g_common_motor_status[common_motor_id].compensate_stage = COMMON_MOTOR_COMPENSATE_READY;
    g_common_motor_status[common_motor_id].compensate_steps = 0;
    // 启动任务参数： mission_common_motor_move
    /* 
        ["$COMMON_MOTOR",     "0", "普通电机ID"],
        ["$MISSION_TYPE",     "0", "任务类型"],
        ["$DETECTOR",         "0", "探测器"],
        ["$DETECTOR_PARAM_1", "0", "探测器启动参数1"],
        ["$ERRCODE",          "0", "探测器未触发的返回码"]
        ["$TIMERID",          "0", "定时器id"],		    
    */
    if(COMMON_MOTOR_MISSION_INIT == mission_type){
        MissionInstanceSetArgs6(mission_id, common_motor_id, mission_type, dect_id, dect_param1, errcode, timer_id);
    }
    else {
        MissionInstanceSetArgs5(mission_id, common_motor_id, mission_type, dect_id, dect_param1, errcode);
    }
    MissionInstanceStart(pPacket, mission_id);

    if (COMMON_MOTOR_MISSION_MOVE_TO_X == mission_type || COMMON_MOTOR_MISSION_MOVE_TO_0 == mission_type){
        // 特殊情况：无需真正执行，立即结束
        if (dest == g_common_motor_status[common_motor_id].current_position){
            common_motor_move_finished(common_motor_id, mission_type, 0);
            return mission_id;
        }
        else if(dest > g_cfg_common_motors[common_motor_id].max_pos_no){
            g_common_motor_status[common_motor_id].cmd_target = g_common_motor_status[common_motor_id].current_position;
            common_motor_move_finished(common_motor_id, mission_type, ERR_MOTOR_UNREACHABLE);
            return mission_id;
        }
    }
    return mission_id;
}

uint_t common_motor_move3(uchar_t* pPacket, uint_t common_motor_id, uint_t mission_type, uint_t dest, uint_t speed, uint_t dir){
    uint_t target = TRANS_TO_TARGET(dir, speed, dest);
    return common_motor_move(pPacket, common_motor_id, mission_type, target);
}

uint_t common_motor_move_steps(uchar_t* pPacket, uint_t common_motor_id, uint_t steps, uint_t speed, uint_t dir){
    g_common_motor_status[common_motor_id].current_position = 0xFF;
    step_motor_move(g_cfg_common_motors[common_motor_id].step_motor_id, dir, speed, steps);
    return 0;
}

// 电机动作结束
uint_t common_motor_move_finished(uint_t common_motor_id, uint_t mission_type, uint_t error_code) {
    uint_t miss_idx = g_cfg_common_motor_mission_map[MISSION_TYPES_NUM*common_motor_id + mission_type];
    if(g_cfg_common_motor_mission[miss_idx].p_done_func){
        g_cfg_common_motor_mission[miss_idx].p_done_func(common_motor_id, mission_type, error_code);
    }
    MissionInstanceDone(g_common_motor_status[common_motor_id].current_mission_id, error_code);
    g_common_motor_status[common_motor_id].current_position = GET_DEST(g_common_motor_status[common_motor_id].cmd_target);
    return 0;
}

// 动作是否完成
uint_t common_motor_is_move_done(uint_t common_motor_id){
    return MissionInstanceIsCompleted(g_common_motor_status[common_motor_id].current_mission_id);
}

// 电机动作是否成功
uint_t common_motor_is_move_succ(uint_t common_motor_id){
    return MissionInstanceIsSucced(g_common_motor_status[common_motor_id].current_mission_id);
}

// 电机动作是否失败
uint_t common_motor_is_move_fail(uint_t common_motor_id){
    return MissionInstanceIsFailed(g_common_motor_status[common_motor_id].current_mission_id);
}

// 普通电机占用和释放
uint_t common_motor_try_lock(uint_t common_motor_id){
    return step_motor_try_lock(g_cfg_common_motors[common_motor_id].step_motor_id);
}

void common_motor_unlock(uint_t common_motor_id){
    step_motor_unlock(g_cfg_common_motors[common_motor_id].step_motor_id);
}

ushort_t common_motor_get_remain_step(uint_t common_motor_id){
    return step_motor_get_remain_step(g_cfg_common_motors[common_motor_id].step_motor_id);
}


ushort_t common_motor_is_stopped(uint_t common_motor_id){
    if(MOTOR_STOPPED_CHECK_BY_NORMAL == g_cfg_common_motors[common_motor_id].stopped_check_type){
        if(0==common_motor_get_remain_step(common_motor_id)){
            return 1;
        }
    }
    return common_motor_extern_is_stopped(common_motor_id);
}


// 获取当前'位置'
uint_t common_motor_get_current_pos(uint_t common_motor_id){
    return g_common_motor_status[common_motor_id].current_position;
}
