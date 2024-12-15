/**********************************************************
filename   : public_missions.h
function   : 任务：复合动作（公共部分）
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#ifndef _PUBLIC_MISSIONS_H_
#define _PUBLIC_MISSIONS_H_

#include "common.h"

// 状态节点（ST）的运行状态
enum NODE_STATUS {
    NODE_DISABLED,           // 沉默状态
    NODE_ACTIVED             // 激活状态
};

// 状态节点类型
enum NODE_TYPE {
    NODE_TYPE_NORMAL,        // 普通节点
    NODE_TYPE_ENTRY,         // 入口节点
    NODE_TYPE_FINAL          // 终止节点
};

// 状态节点的属性配置
typedef struct {
    int              type;                  // 节点的类型
} StatusNodeConf;

// 节点运行状态
typedef struct {
    ushort_t         status;                // 节点的运行状态
    ushort_t         enteredCount;          // 节点进入的次数
} NodeStatus;

// 动作/指示器的参数来源：常量/任务参数
enum ARGS_VALUE_TYPE{
    ARG_FROM_NO_REFERENCE,
    ARG_FROM_MISSION_ARGS
};

typedef struct {
    uint_t id;
    uint_t type;
    uint_t args[6];
    uint_t argTypeSelector;
} Operator;

typedef Operator Action;
typedef Operator Indicator;


// 转换器单元 = StatusNode + Indicators + Actions + NextNode
typedef struct {
    uint_t  node_id;                 // 【配置】节点ID
    uint_t  indct_id;                // 【配置】指示器ID
    Action* pActArr;                 // 【配置】执行动作
    uchar_t act_num;                 // 【配置】动作数量
    uchar_t next_node_num;           // 【配置】本节点执行后的激活节点数量
    uchar_t next_node[6];            // 【配置】本节点执行后的激活节点(最多6个)
} TransformerConf;


// 定义任务 = 状态 + 指示器 + 转换器 + 初始激活节点
typedef struct {
    StatusNodeConf*  pStatusNodeConfArr;    // 【配置】状态节点
    uint_t           node_num;              // 【配置】状态节点数量
    Indicator*       pIndctArr;             // 【配置】指示器
    uint_t           indct_num;             // 【配置】指示器数量
    TransformerConf* pTransformerConfArr;   // 【配置】转换器
    uint_t           trans_num;             // 【配置】转换器数量
} Mission;

// 定义任务执行状态
enum MISSION_STATUS {
    MISSION_INITIALIZED,
    MISSION_EXECUTING,
    MISSION_COMPLITED,
    MISSION_FAILED
};

// 定义任务实例
typedef struct {
    uint_t           my_id;                 // 本任务ID
    Mission*         pMissionRef;           // 任务定义的配置
    NodeStatus*      pNodeStatusArr;        // 存储状态节点运行状态
    int              status;                // 任务执行状态
    uchar_t          cmd_src;               // 任务携带的参数: 发起方
    uchar_t          cmd_dst;               // 任务携带的参数: 目标方
    ushort_t         command;               // 任务携带的参数: 命令ID
    int              errorCode;             // 任务携带的参数: 错误码
    uint_t*          args;                  // 任务参数: 最多32个参数
} MissionInstance;

typedef struct {
    uint_t mission_id;
    MissionInstance* pMissionInst;
} MissionConf;

// 获取任务参数
uint_t MissionGetFunctorArg(MissionInstance* pMission, Operator* pOper, uint_t idx);
// 状态节点的进入和离开
void MissionNodeEnter(MissionInstance* pMission, uint_t nodeId);
void MissionNodeLeave(MissionInstance* pMission, uint_t nodeId);
int MissionNodeIsActived(MissionInstance* pMission, uint_t nodeId);
// 状态节点是否是终态节点
int MissionNodeIsFinally(MissionInstance* pMission, uint_t nodeId);
// 任务重置
void MissionReset(MissionInstance* pMission);
// 任务参数设置
void MissionSetPacketArgs(MissionInstance* pMission, uchar_t src, uchar_t dst, ushort_t cmd);
void MissionSetArgs(MissionInstance* pMission, uint_t* pArgs, uint_t num);

// 任务启动
void MissionStart(MissionInstance* pMission);
// 任务刷新
void MissionTick(MissionInstance* pMission);

// 任务接口
void MissionInstanceSetPacketArgs(uint_t id, uchar_t src, uchar_t dst, ushort_t cmd);
void MissionInstanceSetArgs(uint_t id, uint_t* pArgs, uint_t num);
void MissionInstanceStart(uchar_t* pPacket, uint_t id);
void MissionInstanceDone(uint_t id, uint_t err_code);
void AllMissionsTick(void);
uint_t MissionInstanceGetStatus(uint_t id);
uint_t MissionInstanceIsWorking(uint_t id);
uint_t MissionInstanceIsCompleted(uint_t id);
uint_t MissionInstanceIsFailed(uint_t id);
uint_t MissionInstanceIsSucced(uint_t id);
uint_t MissionInstanceNodeEnteredCountLittleOrEqual(uint_t id, uint_t node_id, int num);
uint_t MissionInstanceNodeEnteredCountEqual(uint_t id, uint_t node_id, int num);
uint_t MissionInstanceNodeEnteredCountBigOrEqual(uint_t id, uint_t node_id, int num);
uint_t MissionInstanceNodeEnteredCountBig(uint_t id, uint_t node_id, int num);
uint_t MissionInstanceNodeEnteredCountLittle(uint_t id, uint_t node_id, int num);

// 任务参数设置
void MissionInstanceSetArgs1(uint_t id, uint_t a1);
void MissionInstanceSetArgs2(uint_t id, uint_t a1, uint_t a2);
void MissionInstanceSetArgs3(uint_t id, uint_t a1, uint_t a2, uint_t a3);
void MissionInstanceSetArgs4(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4);
void MissionInstanceSetArgs5(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5);
void MissionInstanceSetArgs6(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6);
void MissionInstanceSetArgs7(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7);
void MissionInstanceSetArgs8(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8);
void MissionInstanceSetArgs9(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9);
void MissionInstanceSetArgs10(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9, uint_t a10);
void MissionInstanceSetArgs11(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9, uint_t a10, uint_t a11);
void MissionInstanceSetArgs12(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9, uint_t a10, uint_t a11, uint_t a12);

// 发送任务消息
void MissionMessageArgs1(uint_t id, uchar_t arg1);

#endif
