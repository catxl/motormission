/**********************************************************
filename   : mission_demo_action.c
function   : 任务: demo_action
**********************************************************/
#include "mission_inc.h"

// 任务参数说明
//    args[0] : $MUTE : 没用的占位变量 

//=========================================================
// 1. 配置状态节点
//=========================================================
// 状态节点ID定义
enum STATUS_NODE_IDS{
    START, SUCC, NODE_NUM
};
// 状态节点配置数据
static StatusNodeConf g_cfg_status_node[NODE_NUM] = {
    {NODE_TYPE_ENTRY}, {NODE_TYPE_FINAL}
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
// A_FIGHT: 干
static Action A_FIGHT[] = {
    {0,   ACT_NO_OPERATION,             {0, 0, 0, 0, 0, 0},           0},
};
static const int A_FIGHT_NUM = sizeof(A_FIGHT)/sizeof(Action);

//=========================================================
// 3. 配置指示器
//=========================================================
/*
 * 任务的指示器配置
 * 1. 请确保指示器(Indicator)的配置顺序：0~N
*/
// 指示器ID定义
enum INDCT_IDS{
    C_GO, INDCT_IDS_NUM
};
static Indicator g_cfg_mission_idincator[INDCT_IDS_NUM] = {
//   id,                           type,                               args[6],                                                    argTypeSelector
//   ID,                           指示器类型                          指示器的配置参数，由指示器类型而定                          参数值选择器
    {C_GO,                         INDCT_ALWAYS_ACTIVE,                {0, 0, 0, 0, 0, 0},                                          0}, //C_GO: GO
};

//=========================================================
// 4. 配置状态机
//=========================================================
/*
 * 任务状态转换器配置 = StatusNode + Indicators + Actions + NextNode
*/
static TransformerConf g_cfg_mission_transformer[] = {
//   node_id,           indct_id,                     pActArr,            act_num,                      next_node_num,  next_node[6]
    {START,             C_GO,                         A_FIGHT,            A_FIGHT_NUM,                  1,              {SUCC}},  
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
static NodeStatus g_mission_node_status[1][NODE_NUM] = {0};
static uint_t g_miss_inst_args[1][1] = {0};
/*
 * 对外暴露的任务对象
*/
MissionInstance g_mission_inst_demo_action[1] = {
    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[0][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[0][0])
    },
};
