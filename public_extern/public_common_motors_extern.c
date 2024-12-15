/**********************************************************
filename   : public_common_motors_extern.c
function   : 普通电机操作接口（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#include "public_common_motors_extern.h"
#include "public_missions.h"
#include "errcode.h"
#include "public_detectors_extern.h"
#include "sensor_counter_processors.h"
#include "public_utils.h"


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


// 按照不同策略控制电机运动
uint_t common_motor_extern_move_by_strategy(uint_t common_motor_id, uint_t stargety_id, uint_t amount){
    return 0;
}

// 计算普通电机探测器参数
uint_t common_motor_extern_calc_detector_param(uint_t common_motor_id, uint_t mission_type, uint_t mission_arg1){
    uint_t miss_idx = g_cfg_common_motor_mission_map[COMMON_MOTOR_MISSION_EXTERN_TYPES_NUMBER*common_motor_id + mission_type];
    // 根据探测器类型生成探测器启动参数
    switch (g_cfg_detector[g_cfg_common_motor_mission[miss_idx].dect_id].dectType) {
        case DECT_TYPE_SENSOR_COUNTER: {
            uint_t circle_poionts_num = g_cfg_detector[g_cfg_common_motor_mission[miss_idx].dect_id].pDectArgs[0];  // 总的孔位数
            uint_t src_pos = g_common_motor_status[common_motor_id].current_position;
            uint_t dest_pos = GET_DEST(mission_arg1);
            uint_t direction = 0;
            uint_t steps = 0;
            uint_t pos_num = 0;
            get_circle_isometry_pos_move_dir_steps(src_pos, dest_pos, circle_poionts_num, circle_poionts_num, &direction, &steps, &pos_num);
            return 2 * pos_num;
        }
    }
    return mission_arg1;
}

ushort_t common_motor_extern_is_stopped(uint_t common_motor_id){
    return 0;
}
