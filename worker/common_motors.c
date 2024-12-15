/**********************************************************
filename   : common_motors.c
function   : 普通电机
author     : 谢令
created    : 2022-12-05
comment    :
**********************************************************/
#include "common_motors.h"
#include "step_motors.h"
#include "photosensors.h"
#include "detectors.h"
#include "all_missions.h"
#include "errcode.h"
#include "timers.h"
#include "sensor_counter_processors.h"

//旋转电机（环形坐标）点位步数：数量（N）、整圈步数、36533）
static uint_t MOTOR_POS_ROTATE_PLATE[] ={ROTATE_POS_NUMBER, 36533};  
//升降电机位置步数配置//[0]零位,[1]上升目标位
static uint_t MOTOR_POS_CLEANER_SUCKER_VERT[]={0,0x1000};
//升降电机位置步数配置//[0]零位,[1]上升目标位
static uint_t MOTOR_POS_SUBSTRATE_VERT[]={0,0x1000};
//混匀升降电机位置步数配置//[0]零位,[1]上升目标位
static uint_t MOTOR_POS_CLEANER_MIXER_VERT[]={0,0x1000};
//柱塞泵电机位置步数和速度配置: 步数、速度  [0/1]零位,   [2/3]初始化吸液位,  [4/5]吐液位,   [6/7]吸气位
static uint_t MOTOR_POS_SUBSTRATE_PLUNGER[]={0, 13,      400*32, 10,        240*32, 10,   250*32, 3};

// 普通电机细分（步骤）动作配置
COMMON_MOTOR_ACT_STRATEGY_CONF g_cfg_common_motor_sub_actions_strategy[MOTIONS_NUMBER] = {
    // id,                                    strategy_type,                     dir,       speed, steps,  pRef,                           pSpeedRef
    {MOTION_CLEAN_PLATE_ROTATE_INIT_SETP0,    MOVE_STG_DSD_STEPS,                BACKWARD,  0,     0x1000, 0,                              0}, // 旋转电机
    {MOTION_CLEAN_PLATE_ROTATE_INIT_SETP1,    MOVE_STG_DSD_STEPS,                FORWARD,   0,     0xA000, 0,                              0}, // 
    {MOTION_CLEAN_PLATE_ROTATE_TO_X_SETP0,    MOVE_STG_DSX_CIRCLE_ISOMETRY_POS,  0,         2,     0,      MOTOR_POS_ROTATE_PLATE,         0}, // 
    {MOTION_CLEAN_PLATE_ROTATE_TO_X_SETP1,    MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_CLEAN_PLATE_ROTATE_TO_0_SETP0,    MOVE_STG_DSX_CIRCLE_ISOMETRY_POS,  0,         2,     0,      MOTOR_POS_ROTATE_PLATE,         0}, // 
    {MOTION_CLEAN_PLATE_ROTATE_TO_0_SETP1,    MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_CLEANER_SUCKER_VERT_INIT_SETP0,   MOVE_STG_DSD_STEPS,                FORWARD,   0,     0x0300, 0,                              0}, // 升降电机
    {MOTION_CLEANER_SUCKER_VERT_INIT_SETP1,   MOVE_STG_DSD_STEPS,                BACKWARD,  0,     0x1000, 0,                              0}, // 
    {MOTION_CLEANER_SUCKER_VERT_TO_X_SETP0,   MOVE_STG_DSX_POS,                  0,        10,     0,      MOTOR_POS_CLEANER_SUCKER_VERT,  0}, // 
    {MOTION_CLEANER_SUCKER_VERT_TO_X_SETP1,   MOVE_STG_DO_NOTHIN,                0,         0,     0,      0,                              0}, // 
    {MOTION_CLEANER_SUCKER_VERT_TO_0_SETP0,   MOVE_STG_DSX_OFFSET_STEP,          BACKWARD, 10,     0,      0,                              0}, // 
    {MOTION_CLEANER_SUCKER_VERT_TO_0_SETP1,   MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_SUBSTRATE_MIXER_VERT_INIT_SETP0,  MOVE_STG_DSD_STEPS,                FORWARD,   0,     0x0300, 0,                              0}, // 升降电机
    {MOTION_SUBSTRATE_MIXER_VERT_INIT_SETP1,  MOVE_STG_DSD_STEPS,                BACKWARD,  0,     0x1000, 0,                              0}, // 
    {MOTION_SUBSTRATE_MIXER_VERT_TO_X_SETP0,  MOVE_STG_DSX_POS,                  0,         5,     0,      MOTOR_POS_SUBSTRATE_VERT,       0}, // 
    {MOTION_SUBSTRATE_MIXER_VERT_TO_X_SETP1,  MOVE_STG_DO_NOTHIN,                0,         0,     0,      0,                              0}, // 
    {MOTION_SUBSTRATE_MIXER_VERT_TO_0_SETP0,  MOVE_STG_DSX_OFFSET_STEP,          BACKWARD,  5,     0,      0,                              0}, // 
    {MOTION_SUBSTRATE_MIXER_VERT_TO_0_SETP1,  MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_SUBSTRATE_A_PLUNGER_INIT_SETP0,   MOVE_STG_DSD_STEPS,                FORWARD,   0,     0x1F40, 0,                              0}, // 柱塞泵电机
    {MOTION_SUBSTRATE_A_PLUNGER_INIT_SETP1,   MOVE_STG_DSD_STEPS,                BACKWARD,  0,     0x7D00, 0,                              0}, // 
    {MOTION_SUBSTRATE_A_PLUNGER_TO_X_SETP0,   MOVE_STG_XXX_POS_SPEED,            0,         0,     0,      MOTOR_POS_SUBSTRATE_PLUNGER,    0}, // 
    {MOTION_SUBSTRATE_A_PLUNGER_TO_X_SETP1,   MOVE_STG_DO_NOTHIN,                0,         0,     0,      0,                              0}, // 
    {MOTION_SUBSTRATE_A_PLUNGER_TO_0_SETP0,   MOVE_STG_DSX_OFFSET_STEP,          BACKWARD,  0,     0,      0,                              0}, // 
    {MOTION_SUBSTRATE_A_PLUNGER_TO_0_SETP1,   MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_SUBSTRATE_B_PLUNGER_INIT_SETP0,   MOVE_STG_DSD_STEPS,                FORWARD,   0,     0x1F40, 0,                              0}, // 柱塞泵电机
    {MOTION_SUBSTRATE_B_PLUNGER_INIT_SETP1,   MOVE_STG_DSD_STEPS,                BACKWARD,  0,     0x7D00, 0,                              0}, // 
    {MOTION_SUBSTRATE_B_PLUNGER_TO_X_SETP0,   MOVE_STG_XXX_POS_SPEED,            0,         0,     0,      MOTOR_POS_SUBSTRATE_PLUNGER,    0}, // 
    {MOTION_SUBSTRATE_B_PLUNGER_TO_X_SETP1,   MOVE_STG_DO_NOTHIN,                0,         0,     0,      0,                              0}, // 
    {MOTION_SUBSTRATE_B_PLUNGER_TO_0_SETP0,   MOVE_STG_DSX_OFFSET_STEP,          BACKWARD,  0,     0,      0,                              0}, // 
    {MOTION_SUBSTRATE_B_PLUNGER_TO_0_SETP1,   MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_CLEANER_MIXER_VERT_INIT_SETP0,    MOVE_STG_DSD_STEPS,                FORWARD,   0,     0x0300, 0,                              0}, // 混匀升降电机
    {MOTION_CLEANER_MIXER_VERT_INIT_SETP1,    MOVE_STG_DSD_STEPS,                BACKWARD,  0,     0x1000, 0,                              0}, // 
    {MOTION_CLEANER_MIXER_VERT_TO_X_SETP0,    MOVE_STG_DSX_POS,                  0,         5,     0,      MOTOR_POS_CLEANER_MIXER_VERT,   0}, // 
    {MOTION_CLEANER_MIXER_VERT_TO_X_SETP1,    MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
    {MOTION_CLEANER_MIXER_VERT_TO_0_SETP0,    MOVE_STG_DSX_OFFSET_STEP,          BACKWARD,  5,     0,      0,                              0}, // 
    {MOTION_CLEANER_MIXER_VERT_TO_0_SETP1,    MOVE_STG_XSD_KEEP_DIR,             0,         0,     0x0100, 0,                              0}, // 
};

static uint_t s_p_ACT_STG_CLEAN_PLATE_ROTATE_INIT   [] = { MOTION_CLEAN_PLATE_ROTATE_INIT_SETP0   ,  MOTION_CLEAN_PLATE_ROTATE_INIT_SETP1   };
static uint_t s_p_ACT_STG_CLEAN_PLATE_ROTATE_TO_X   [] = { MOTION_CLEAN_PLATE_ROTATE_TO_X_SETP0   ,  MOTION_CLEAN_PLATE_ROTATE_TO_X_SETP1   };
//static uint_t s_p_ACT_STG_CLEAN_PLATE_ROTATE_TO_0 [] = { MOTION_CLEAN_PLATE_ROTATE_TO_0_SETP0   ,  MOTION_CLEAN_PLATE_ROTATE_TO_0_SETP1   };
static uint_t s_p_ACT_STG_CLEANER_SUCKER_VERT_INIT  [] = { MOTION_CLEANER_SUCKER_VERT_INIT_SETP0  ,  MOTION_CLEANER_SUCKER_VERT_INIT_SETP1  };
static uint_t s_p_ACT_STG_CLEANER_SUCKER_VERT_TO_X  [] = { MOTION_CLEANER_SUCKER_VERT_TO_X_SETP0  ,  MOTION_CLEANER_SUCKER_VERT_TO_X_SETP1  };
static uint_t s_p_ACT_STG_CLEANER_SUCKER_VERT_TO_0  [] = { MOTION_CLEANER_SUCKER_VERT_TO_0_SETP0  ,  MOTION_CLEANER_SUCKER_VERT_TO_0_SETP1  };
static uint_t s_p_ACT_STG_SUBSTRATE_MIXER_VERT_INIT [] = { MOTION_SUBSTRATE_MIXER_VERT_INIT_SETP0 ,  MOTION_SUBSTRATE_MIXER_VERT_INIT_SETP1 };
static uint_t s_p_ACT_STG_SUBSTRATE_MIXER_VERT_TO_X [] = { MOTION_SUBSTRATE_MIXER_VERT_TO_X_SETP0 ,  MOTION_SUBSTRATE_MIXER_VERT_TO_X_SETP1 };
static uint_t s_p_ACT_STG_SUBSTRATE_MIXER_VERT_TO_0 [] = { MOTION_SUBSTRATE_MIXER_VERT_TO_0_SETP0 ,  MOTION_SUBSTRATE_MIXER_VERT_TO_0_SETP1 };
static uint_t s_p_ACT_STG_SUBSTRATE_A_PLUNGER_INIT  [] = { MOTION_SUBSTRATE_A_PLUNGER_INIT_SETP0  ,  MOTION_SUBSTRATE_A_PLUNGER_INIT_SETP1  };
static uint_t s_p_ACT_STG_SUBSTRATE_A_PLUNGER_TO_X  [] = { MOTION_SUBSTRATE_A_PLUNGER_TO_X_SETP0  ,  MOTION_SUBSTRATE_A_PLUNGER_TO_X_SETP1  };
static uint_t s_p_ACT_STG_SUBSTRATE_A_PLUNGER_TO_0  [] = { MOTION_SUBSTRATE_A_PLUNGER_TO_0_SETP0  ,  MOTION_SUBSTRATE_A_PLUNGER_TO_0_SETP1  };
static uint_t s_p_ACT_STG_SUBSTRATE_B_PLUNGER_INIT  [] = { MOTION_SUBSTRATE_B_PLUNGER_INIT_SETP0  ,  MOTION_SUBSTRATE_B_PLUNGER_INIT_SETP1  };
static uint_t s_p_ACT_STG_SUBSTRATE_B_PLUNGER_TO_X  [] = { MOTION_SUBSTRATE_B_PLUNGER_TO_X_SETP0  ,  MOTION_SUBSTRATE_B_PLUNGER_TO_X_SETP1  };
static uint_t s_p_ACT_STG_SUBSTRATE_B_PLUNGER_TO_0  [] = { MOTION_SUBSTRATE_B_PLUNGER_TO_0_SETP0  ,  MOTION_SUBSTRATE_B_PLUNGER_TO_0_SETP1  };
static uint_t s_p_ACT_STG_CLEANER_MIXER_VERT_INIT   [] = { MOTION_CLEANER_MIXER_VERT_INIT_SETP0   ,  MOTION_CLEANER_MIXER_VERT_INIT_SETP1   };
static uint_t s_p_ACT_STG_CLEANER_MIXER_VERT_TO_X   [] = { MOTION_CLEANER_MIXER_VERT_TO_X_SETP0   ,  MOTION_CLEANER_MIXER_VERT_TO_X_SETP1   };
static uint_t s_p_ACT_STG_CLEANER_MIXER_VERT_TO_0   [] = { MOTION_CLEANER_MIXER_VERT_TO_0_SETP0   ,  MOTION_CLEANER_MIXER_VERT_TO_0_SETP1   };

//=================================================================
/*
 * 本单元使用到的普通电机的实现定义
*/
const uint_t G_COMMON_MOTORS_NUMBER = COMMON_MOTORS_NUMBER;              // 普通电机的数量MOTOR_STOPPED_CHECK_BY_NORMAL
COMMON_MOTOR_CONF g_cfg_common_motors[COMMON_MOTORS_NUMBER] = {          // 普通电机配置参数
//   step_motor_id,                      step_motor_id,                      max_pos,  stopped_check_type,             stopped_check_param
    {COMMON_MOTOR_CLEAN_PLATE_ROTATE,    STEP_MOTOR_CLEAN_PLATE_ROTATE_M0,        71,  MOTOR_STOPPED_CHECK_BY_NORMAL,  0},// 旋转电机
    {COMMON_MOTOR_CLEANER_SUCKER_VERT,   STEP_MOTOR_CLEANER_SUCKER_VERT_M1,        1,  MOTOR_STOPPED_CHECK_BY_NORMAL,  0},// 升降电机
    {COMMON_MOTOR_SUBSTRATE_MIXER_VERT,  STEP_MOTOR_SUBSTRATE_MIXER_VERT_M2,       1,  MOTOR_STOPPED_CHECK_BY_NORMAL,  0},// 混匀升降电机
    {COMMON_MOTOR_SUBSTRATE_A_PLUNGER,   STEP_MOTOR_SUBSTRATE_A_PLUNGER_M11,       3,  MOTOR_STOPPED_CHECK_BY_NORMAL,  0},// 柱塞泵电机
    {COMMON_MOTOR_SUBSTRATE_B_PLUNGER,   STEP_MOTOR_SUBSTRATE_B_PLUNGER_M12,       3,  MOTOR_STOPPED_CHECK_BY_NORMAL,  0},// 柱塞泵电机
    {COMMON_MOTOR_CLEANER_MIXER_VERT,    STEP_MOTOR_CLEANER_MIXER_VERT_M13,        1,  MOTOR_STOPPED_CHECK_BY_NORMAL,  0},// 混匀升降电机
};

//=================================================================
// 任务完成函数实现定义
// 1. 重置步进电机的offset
static void s_pf_reset_offset(uint_t common_motor_id, uint_t mission_type, uint_t error_code){
    step_motor_reset_offset_steps(g_cfg_common_motors[common_motor_id].step_motor_id);
}
// 2. 重置旋转电机编码器
static void s_pf_reset_encoder(uint_t common_motor_id, uint_t mission_type, uint_t error_code){
    step_motor_reset_offset_steps(g_cfg_common_motors[common_motor_id].step_motor_id);
    SensorCounterProcessingReset(SENSOR_COUNTER_CLEAN_PLATE_ROTATE);
}

// 普通电机动作的任务参数配置
COMMON_MOTOR_MISSION_CONF g_cfg_common_motor_mission[MOTOR_ACTIONS_NUMBER] = {
//   common_motor_act_id,                 mission_id,                         p_done_func,        dect_id,                                               errcode,                p_act_stg_step_arr                      compensate, timer_id
    {MOVE_ACT_CLEAN_PLATE_ROTATE_INIT,    MISSION_CLEAN_PLATE_ROTATE_INIT,    s_pf_reset_encoder, DETECTOR_PHOTOSENSOR_CLEAN_PLATE_ROTATE_POS0_S0,       ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_CLEAN_PLATE_ROTATE_INIT,    0x0120,     TIMER_DELAY_MOTOR_CLEAN_PLATE_ROTATE},// 旋转电机
    {MOVE_ACT_CLEAN_PLATE_ROTATE_TO_X,    MISSION_CLEAN_PLATE_ROTATE_TO_X,    0,                  DETECTOR_MOTOR_PLATE_ROTATE_PULSE_COUNTER,             0,                      s_p_ACT_STG_CLEAN_PLATE_ROTATE_TO_X,    0x0090,     0},// 
    {MOVE_ACT_CLEAN_PLATE_ROTATE_TO_0,    MISSION_CLEAN_PLATE_ROTATE_TO_0,    0,                  DETECTOR_MOTOR_PLATE_ROTATE_PULSE_COUNTER,             0,                      s_p_ACT_STG_CLEAN_PLATE_ROTATE_TO_X,    0x0090,     0},// 
    {MOVE_ACT_CLEANER_SUCKER_VERT_INIT,   MISSION_CLEANER_SUCKER_VERT_INIT,   s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_CLEANER_SUCKER_VERT_RESET_S2,     ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_CLEANER_SUCKER_VERT_INIT,   0x0020,     TIMER_DELAY_MOTOR_CLEANER_SUCKER_VERT},// 升降电机
    {MOVE_ACT_CLEANER_SUCKER_VERT_TO_X,   MISSION_CLEANER_SUCKER_VERT_TO_X,   0,                  DETECTOR_REJECT,                                       0,                      s_p_ACT_STG_CLEANER_SUCKER_VERT_TO_X,        0,     0},// 
    {MOVE_ACT_CLEANER_SUCKER_VERT_TO_0,   MISSION_CLEANER_SUCKER_VERT_TO_0,   s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_CLEANER_SUCKER_VERT_RESET_S2,     ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_CLEANER_SUCKER_VERT_TO_0,   0x0020,     0},// 
    {MOVE_ACT_SUBSTRATE_MIXER_VERT_INIT,  MISSION_SUBSTRATE_MIXER_VERT_INIT,  s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_SUBSTRATE_MIXER_VERT_LOW_POS_S6,  ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_SUBSTRATE_MIXER_VERT_INIT,  0x0020,     TIMER_DELAY_MOTOR_SUBSTRATE_MIXER_VERT},// 混匀升降电机
    {MOVE_ACT_SUBSTRATE_MIXER_VERT_TO_X,  MISSION_SUBSTRATE_MIXER_VERT_TO_X,  0,                  DETECTOR_REJECT,                                       0,                      s_p_ACT_STG_SUBSTRATE_MIXER_VERT_TO_X,       0,     0},// 
    {MOVE_ACT_SUBSTRATE_MIXER_VERT_TO_0,  MISSION_SUBSTRATE_MIXER_VERT_TO_0,  s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_SUBSTRATE_MIXER_VERT_LOW_POS_S6,  ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_SUBSTRATE_MIXER_VERT_TO_0,  0x0020,     0},// 
    {MOVE_ACT_SUBSTRATE_A_PLUNGER_INIT,   MISSION_SUBSTRATE_A_PLUNGER_INIT,   s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_SUBSTRATE_A_PLUNGER_S14,          ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_SUBSTRATE_A_PLUNGER_INIT,   0x0020,     TIMER_DELAY_MOTOR_SUBSTRATE_A_PLUNGER},// 柱塞泵电机
    {MOVE_ACT_SUBSTRATE_A_PLUNGER_TO_X,   MISSION_SUBSTRATE_A_PLUNGER_TO_X,   0,                  DETECTOR_REJECT,                                       0,                      s_p_ACT_STG_SUBSTRATE_A_PLUNGER_TO_X,        0,     0},// 
    {MOVE_ACT_SUBSTRATE_A_PLUNGER_TO_0,   MISSION_SUBSTRATE_A_PLUNGER_TO_0,   s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_SUBSTRATE_A_PLUNGER_S14,          ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_SUBSTRATE_A_PLUNGER_TO_0,   0x0020,     0},// 
    {MOVE_ACT_SUBSTRATE_B_PLUNGER_INIT,   MISSION_SUBSTRATE_B_PLUNGER_INIT,   s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_SUBSTRATE_B_PLUNGER_S15,          ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_SUBSTRATE_B_PLUNGER_INIT,   0x0020,     TIMER_DELAY_MOTOR_SUBSTRATE_B_PLUNGER},// 柱塞泵电机
    {MOVE_ACT_SUBSTRATE_B_PLUNGER_TO_X,   MISSION_SUBSTRATE_B_PLUNGER_TO_X,   0,                  DETECTOR_REJECT,                                       0,                      s_p_ACT_STG_SUBSTRATE_B_PLUNGER_TO_X,        0,     0},// 
    {MOVE_ACT_SUBSTRATE_B_PLUNGER_TO_0,   MISSION_SUBSTRATE_B_PLUNGER_TO_0,   s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_SUBSTRATE_B_PLUNGER_S15,          ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_SUBSTRATE_B_PLUNGER_TO_0,   0x0020,     0},// 
    {MOVE_ACT_CLEANER_MIXER_VERT_INIT,    MISSION_CLEANER_MIXER_VERT_INIT,    s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_LOW_POS_S4,    ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_CLEANER_MIXER_VERT_INIT,    0x0030,     TIMER_DELAY_MOTOR_CLEANER_MIXER_VERT},// 混匀升降电机
    {MOVE_ACT_CLEANER_MIXER_VERT_TO_X,    MISSION_CLEANER_MIXER_VERT_TO_X,    0,                  DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_HIGH_POS_S3,   ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_CLEANER_MIXER_VERT_TO_X,    0x0030,     0},// 
    {MOVE_ACT_CLEANER_MIXER_VERT_TO_0,    MISSION_CLEANER_MIXER_VERT_TO_0,    s_pf_reset_offset,  DETECTOR_PHOTOSENSOR_CLEANER_MIXER_VERT_LOW_POS_S4,    ERR_MOTOR_NOT_REACHED,  s_p_ACT_STG_CLEANER_MIXER_VERT_TO_0,    0x0030,     0},// 
};

// 普通电机的 【普通电机ID + 任务类型】 --> 任务参数配置  映射表
uint_t g_cfg_common_motor_mission_map[COMMON_MOTORS_NUMBER][COMMON_MOTOR_MISSION_EXTERN_TYPES_NUMBER] = {  // 配置普通电机移动策略参数，分为Init/To(0)/To(pos)
    /*COMMON_MOTOR_CLEAN_PLATE_ROTATE,   */{MOVE_ACT_CLEAN_PLATE_ROTATE_INIT,    MOVE_ACT_CLEAN_PLATE_ROTATE_TO_0,    MOVE_ACT_CLEAN_PLATE_ROTATE_TO_X  },// 旋转电机
    /*COMMON_MOTOR_CLEANER_SUCKER_VERT,  */{MOVE_ACT_CLEANER_SUCKER_VERT_INIT,   MOVE_ACT_CLEANER_SUCKER_VERT_TO_0,   MOVE_ACT_CLEANER_SUCKER_VERT_TO_X },// 升降电机
    /*COMMON_MOTOR_SUBSTRATE_MIXER_VERT, */{MOVE_ACT_SUBSTRATE_MIXER_VERT_INIT,  MOVE_ACT_SUBSTRATE_MIXER_VERT_TO_0,  MOVE_ACT_SUBSTRATE_MIXER_VERT_TO_X},// 混匀升降电机
    /*COMMON_MOTOR_SUBSTRATE_A_PLUNGER,  */{MOVE_ACT_SUBSTRATE_A_PLUNGER_INIT,   MOVE_ACT_SUBSTRATE_A_PLUNGER_TO_0,   MOVE_ACT_SUBSTRATE_A_PLUNGER_TO_X },// 柱塞泵电机
    /*COMMON_MOTOR_SUBSTRATE_B_PLUNGER,  */{MOVE_ACT_SUBSTRATE_B_PLUNGER_INIT,   MOVE_ACT_SUBSTRATE_B_PLUNGER_TO_0,   MOVE_ACT_SUBSTRATE_B_PLUNGER_TO_X },// 柱塞泵电机
    /*COMMON_MOTOR_CLEANER_MIXER_VERT,   */{MOVE_ACT_CLEANER_MIXER_VERT_INIT,    MOVE_ACT_CLEANER_MIXER_VERT_TO_0,    MOVE_ACT_CLEANER_MIXER_VERT_TO_X  },// 混匀升降电机
};

COMMON_MOTOR_RUNTIME_STATUS g_common_motor_status[COMMON_MOTORS_NUMBER] = { 0 };
//=================================================================

