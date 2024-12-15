/**********************************************************
filename   : mission_common_motor_move.c
function   : 任务: common_motor_move
**********************************************************/
#include "mission_inc.h"

// 任务参数说明
//    args[0] : $COMMON_MOTOR : 普通电机ID 
//    args[1] : $MISSION_TYPE : 任务类型 
//    args[2] : $DETECTOR : 探测器 
//    args[3] : $DETECTOR_PARAM_1 : 探测器启动参数1 
//    args[4] : $ERRCODE : 探测器未触发的返回码 

//=========================================================
// 1. 配置状态节点
//=========================================================
// 状态节点ID定义
enum STATUS_NODE_IDS{
    START, ST_READY, ST_TARGETING, ST_COMPENSATE, ST_AGAIN, ST_CODED_END, SUCC, NODE_NUM
};
// 状态节点配置数据
static StatusNodeConf g_cfg_status_node[NODE_NUM] = {
    {NODE_TYPE_ENTRY}, {NODE_TYPE_NORMAL}, {NODE_TYPE_NORMAL}, {NODE_TYPE_NORMAL}, {NODE_TYPE_NORMAL}, {NODE_TYPE_FINAL}, {NODE_TYPE_FINAL}
};
static const uint_t g_cfg_status_node_num = NODE_NUM;
    

//=========================================================
// 2. 配置动作
//=========================================================
/*
 * 执行动作
 * 动作参数(参考：public_actions.c)
 *    伐操作，参数序号: 0(on/off)，1(阀的数量)，2-5阀的ID
 *    开关操作，参数序号: 0(on/off)，1(开关的ID)
 *    启动倒计时，参数序号: 0(计时器的ID)，1(超时ms)
 *    步进电机转动，参数序号: 0(步进电机的ID)，1(方向)，2(速度)，3(步数)
 *    混匀电机转动，参数序号: 0(电机ID)，1(速度)
 *    修改节点状态，参数序号: 0(任务ID)，1(节点ID)，2(状态值)
 *    任务完成，参数序号: 0(任务ID)
 *    任务失败，参数序号: 0(任务ID)，1(错误码)
 *    自定义执行函数，参数序号: 0(函数入口地址)
*/
// A_NO_OP: 静观其变
static Action A_NO_OP[] = {
    {0,   ACT_NO_OPERATION,             {0, 0, 0, 0, 0, 0},           0},
};
static const int A_NO_OP_NUM = sizeof(A_NO_OP)/sizeof(Action);
// A_TARGET: 启动探测器，电机向目标方向移动
static Action A_TARGET[] = {
    {0,   ACT_DETECTOR_START,           {2, 3, 0, 0, 0, 0},           (1<<0)|(1<<1)},
    {1,   ACT_COMMON_MOTOR_MOVE_STEP_X, {0, 1, 0, 0, 0, 0},           (1<<0)|(1<<1)},
};
static const int A_TARGET_NUM = sizeof(A_TARGET)/sizeof(Action);
// A_AGAIN: 电机补偿移动1
static Action A_AGAIN[] = {
    {0,   ACT_COMMON_MOTOR_MOVE_STEP_X, {0, 1, 1, 0, 0, 0},           (1<<0)|(1<<1)},
};
static const int A_AGAIN_NUM = sizeof(A_AGAIN)/sizeof(Action);
// A_COMPENSATE: 电机补偿移动2
static Action A_COMPENSATE[] = {
    {0,   ACT_COMMON_MOTOR_MV_COMPENSATE,{0, 1, 0, 0, 0, 0},           (1<<0)|(1<<1)},
};
static const int A_COMPENSATE_NUM = sizeof(A_COMPENSATE)/sizeof(Action);
// A_QUICK_CLEAR: 释放电机，操作结束
static Action A_QUICK_CLEAR[] = {
    {0,   ACT_COMMON_MOTOR_RELEASE,     {0, 0, 0, 0, 0, 0},           (1<<0)},
    {1,   ACT_COMMON_MOTOR_DONE,        {0, 1, 0, 0, 0, 0},           (1<<0)|(1<<1)},
};
static const int A_QUICK_CLEAR_NUM = sizeof(A_QUICK_CLEAR)/sizeof(Action);
// A_CODED_CLEAR: 停止探测器，释放电机，操作结束
static Action A_CODED_CLEAR[] = {
    {0,   ACT_DETECTOR_STOP,            {2, 0, 0, 0, 0, 0},           (1<<0)},
    {1,   ACT_COMMON_MOTOR_RELEASE,     {0, 0, 0, 0, 0, 0},           (1<<0)},
    {2,   ACT_COMMON_MOTOR_DONE,        {0, 1, 4, 0, 0, 0},           (1<<0)|(1<<1)|(1<<2)},
};
static const int A_CODED_CLEAR_NUM = sizeof(A_CODED_CLEAR)/sizeof(Action);
// A_CLEAR: 停止探测器，释放电机，操作成功
static Action A_CLEAR[] = {
    {0,   ACT_DETECTOR_STOP,            {2, 0, 0, 0, 0, 0},           (1<<0)},
    {1,   ACT_COMMON_MOTOR_RELEASE,     {0, 0, 0, 0, 0, 0},           (1<<0)},
    {2,   ACT_COMMON_MOTOR_DONE,        {0, 1, 0, 0, 0, 0},           (1<<0)|(1<<1)},
};
static const int A_CLEAR_NUM = sizeof(A_CLEAR)/sizeof(Action);

//=========================================================
// 3. 配置指示器
//=========================================================
/*
 * 任务的指示器配置
 * 1. 请确保指示器(Indicator)的配置顺序：0~N
*/
// 指示器ID定义
enum INDCT_IDS{
    C_TRY_LOCK, C_MOVE_DONE, C_ACTIVE, C_NOT_ACT, C_COMPENSATE_DONE, C_NOT_MEET_DETECTOR, INDCT_IDS_NUM
};
static Indicator g_cfg_mission_idincator[INDCT_IDS_NUM] = {
//   id,                           type,                               args[6],                                                    argTypeSelector
//   ID,                           指示器类型                          指示器的配置参数，由指示器类型而定                          参数值选择器
    {C_TRY_LOCK,                   INDCT_COMMON_MOTOR_GOTIT_IF_STOPED, {0, 0, 0, 0, 0, 0},                                          (1<<0)}, //C_TRY_LOCK: $COMMON_MOTOR 电机使用权获取成功
    {C_MOVE_DONE,                  INDCT_COMMON_MOTOR_STOPPED,         {0, 0, 0, 0, 0, 0},                                          (1<<0)}, //C_MOVE_DONE: $COMMON_MOTOR 电机运动完成
    {C_ACTIVE,                     INDCT_DETECTOR_TRIGGERED,           {2, 0, 0, 0, 0, 0},                                          (1<<0)}, //C_ACTIVE: $DETECTOR 探测器激活
    {C_NOT_ACT,                    INDCT_DETECTOR_NOT_TRIGGERED,       {2, 0, 0, 0, 0, 0},                                          (1<<0)}, //C_NOT_ACT: $DETECTOR 探测器未激活
    {C_COMPENSATE_DONE,            INDCT_COMMON_MOTOR_COMPENSATE_DONE, {0, 0, 0, 0, 0, 0},                                          (1<<0)}, //C_COMPENSATE_DONE: $COMMON_MOTOR 电机补偿运动完成
    {C_NOT_MEET_DETECTOR,          INDCT_LOGIC_AND,                    {2, C_MOVE_DONE, C_NOT_ACT, 0, 0, 0},                        0}, //C_NOT_MEET_DETECTOR: 走完了探测器也未触发
};

//=========================================================
// 4. 配置状态机
//=========================================================
/*
 * 任务状态转换器配置 = StatusNode + Indicators + Actions + NextNode
*/
static TransformerConf g_cfg_mission_transformer[] = {
//   node_id,           indct_id,                     pActArr,            act_num,                      next_node_num,  next_node[6]
    {START,             C_TRY_LOCK,                   A_NO_OP,            A_NO_OP_NUM,                  1,              {ST_READY}},
    {ST_READY,          C_ACTIVE,                     A_QUICK_CLEAR,      A_QUICK_CLEAR_NUM,            1,              {SUCC}},  
    {ST_READY,          C_NOT_ACT,                    A_TARGET,           A_TARGET_NUM,                 1,              {ST_TARGETING}},
    {ST_TARGETING,      C_NOT_MEET_DETECTOR,          A_AGAIN,            A_AGAIN_NUM,                  1,              {ST_AGAIN}},
    {ST_TARGETING,      C_ACTIVE,                     A_COMPENSATE,       A_COMPENSATE_NUM,             1,              {ST_COMPENSATE}},
    {ST_AGAIN,          C_NOT_MEET_DETECTOR,          A_CODED_CLEAR,      A_CODED_CLEAR_NUM,            1,              {ST_CODED_END}},
    {ST_AGAIN,          C_ACTIVE,                     A_COMPENSATE,       A_COMPENSATE_NUM,             1,              {ST_COMPENSATE}},
    {ST_COMPENSATE,     C_COMPENSATE_DONE,            A_CLEAR,            A_CLEAR_NUM,                  1,              {SUCC}},  
};
static const uint_t g_cfg_missions_trans_num = sizeof(g_cfg_mission_transformer)/sizeof(TransformerConf);


//=========================================================
// 5. 配置任务（此处配完之后，还需在all_missions.h/.c中配置）
//=========================================================
// 定义任务 = 状态 + 指示器 + 转换器 + 初始激活节点
static Mission g_cfg_mission = {
    g_cfg_status_node,                      // 【配置】状态节点
    g_cfg_status_node_num,                  // 【配置】状态节点数量
    g_cfg_mission_idincator,                // 【配置】指示器
    INDCT_IDS_NUM,                          // 【配置】指示器数量
    g_cfg_mission_transformer,              // 【配置】转换器
    g_cfg_missions_trans_num,               // 【配置】转换器数量
};
// 记录任务内部运行状态的内存变量
static NodeStatus g_mission_node_status[6][NODE_NUM] = {0};
static uint_t g_miss_inst_args[6][5] = {0};
/*
 * 对外暴露的任务对象
*/
MissionInstance g_mission_inst_common_motor_move[6] = {
    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[0][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[0][0])
    },    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[1][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[1][0])
    },    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[2][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[2][0])
    },    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[3][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[3][0])
    },    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[4][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[4][0])
    },    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[5][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[5][0])
    },
};
