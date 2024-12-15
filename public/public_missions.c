/**********************************************************
filename   : public_missions.c
function   : 任务：复合动作（公共部分）
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#include "public_missions.h"
#include "public_indicators.h"
#include "public_actions.h"
#include "errcode.h"
#include "cmd_proc_func.h"
#include "byte_sequence.h"
#include "commpacket.h"
#include "utils.h"

extern MissionConf g_cfg_mission[];
extern const int G_MISSION_NUM;

// 获取任务参数
uint_t MissionGetFunctorArg(MissionInstance* pMission, Operator* pOper, uint_t idx){
    if(0==pOper->argTypeSelector || ARG_FROM_NO_REFERENCE == (pOper->argTypeSelector & (1<<idx))){
        return pOper->args[idx];
    }
    return pMission->args[pOper->args[idx]];
}

// 设置节点状态
void MissionNodeEnter(MissionInstance* pMission, uint_t nodeId){
    pMission->pNodeStatusArr[nodeId].status = NODE_ACTIVED;
    pMission->pNodeStatusArr[nodeId].enteredCount += 1;
    SEND_MISSION_TRACE_MSG(pMission->my_id, nodeId);
}
void MissionNodeLeave(MissionInstance* pMission, uint_t nodeId){
    pMission->pNodeStatusArr[nodeId].status = NODE_DISABLED;
}
int MissionNodeIsActived(MissionInstance* pMission, uint_t nodeId){
    if(NODE_ACTIVED == pMission->pNodeStatusArr[nodeId].status 
       && (!MissionNodeIsFinally(pMission, nodeId)) ){
        return 1;
    }
    return 0;
}
// 状态节点是否是终态节点
int MissionNodeIsFinally(MissionInstance* pMission, uint_t nodeId){
    if(NODE_TYPE_FINAL == pMission->pMissionRef->pStatusNodeConfArr[nodeId].type){
        return 1;
    }
    return 0;
}

void MissionReset(MissionInstance* pMission){
    uint_t i = 0;
    for(i=0; i<pMission->pMissionRef->node_num; i++){
        pMission->pNodeStatusArr[i].status = NODE_DISABLED;
        pMission->pNodeStatusArr[i].enteredCount = 0;
    }
    pMission->status = MISSION_INITIALIZED;
}

void MissionSetPacketArgs(MissionInstance* pMission, uchar_t src, uchar_t dst, ushort_t cmd){
    pMission->cmd_src = src;
    pMission->cmd_dst = dst;
    pMission->command = cmd;
}

void MissionSetArgs(MissionInstance* pMission, uint_t* pArgs, uint_t num){
    uint_t i = 0;
    for(i=0; i<num; i++){
        pMission->args[i] = pArgs[i];
    }
}

void MissionStart(MissionInstance* pMission){
    uint_t i = 0;
    MissionReset(pMission);
    pMission->status = MISSION_EXECUTING;
    for(i=0; i<pMission->pMissionRef->node_num; i++){
        if(NODE_TYPE_ENTRY == pMission->pMissionRef->pStatusNodeConfArr[i].type){
            MissionNodeEnter(pMission, i);
        }
    }
}

void MissionTick(MissionInstance * pMission){
    //
    // 0. 只处理处于激活态的节点
    // 1. 从头开始遍历每一个感应态节点的指示器
    // 2. 执行指示器的Show调用
    // 3. 如果Show调用返回1，那么执行对应的动作(Action)，并唤醒后续依赖的指示器开始工作
    //
    uint_t i = 0;
    uint_t j = 0;
    for(i=0; i<pMission->pMissionRef->trans_num; i++){
        TransformerConf* pTransNodeConf = &(pMission->pMissionRef->pTransformerConfArr[i]);
        if(!MissionNodeIsActived(pMission, pTransNodeConf->node_id)){
            continue;
        }
        if(IndicatorShow(pMission, pMission->pMissionRef->pIndctArr, pTransNodeConf->indct_id)>0){
            ActionBatchExecute(pMission, pTransNodeConf->pActArr, pTransNodeConf->act_num);
            MissionNodeLeave(pMission, pTransNodeConf->node_id);
            for (j = 0; j < pTransNodeConf->next_node_num; j++){
                MissionNodeEnter(pMission, pTransNodeConf->next_node[j]);
                if (MissionNodeIsFinally(pMission, pTransNodeConf->next_node[j])){
                    pMission->status = MISSION_COMPLITED;
                    return;
                }
            }
        }
    }
}


void MissionInstanceSetPacketArgs(uint_t id, uchar_t src, uchar_t dst, ushort_t cmd){
    MissionSetPacketArgs(g_cfg_mission[id].pMissionInst, src, dst, cmd);
}

void MissionInstanceSetArgs(uint_t id, uint_t* pArgs, uint_t num){
    MissionSetArgs(g_cfg_mission[id].pMissionInst, pArgs, num);
}

void MissionInstanceStart(uchar_t* pPacket, uint_t id){
    MissionInstance* pInst = g_cfg_mission[id].pMissionInst;
    if(MissionInstanceIsWorking(id)){
        if(pPacket){
            SendErrorResponsePacket(pPacket, ERR_MISSION_CONFLICT);
        }
        return;
    }
    MissionReset(pInst);
    pInst->my_id = id;
    if(pPacket){
        MissionInstanceSetPacketArgs(id, GetPackFieldSrc(pPacket), GetPackFieldDst(pPacket), GetPackFieldCmd(pPacket));
    }
    else{
        MissionInstanceSetPacketArgs(id, 0, 0, 0);
    }
    MissionStart(pInst);
}

void MissionInstanceDone(uint_t id, uint_t err_code){
    MissionInstance* pInst = g_cfg_mission[id].pMissionInst;
    g_cfg_mission[id].pMissionInst->status = MISSION_COMPLITED;
    g_cfg_mission[id].pMissionInst->errorCode = err_code;
    if(pInst->command){      // 只有"带命令"的任务才需要发送响应
        uchar_t err[2] = {0};
        uchar_t ctrl = 0xA5;
        ushort_to_big_endian(err_code, err);
        if(err_code>0){
            ctrl = 0xF5;
        }
        SendPacket(ctrl, 0x06, pInst->cmd_dst, pInst->cmd_src, pInst->command, err, 2);
    }
}

uint_t MissionInstanceGetStatus(uint_t id){
    return g_cfg_mission[id].pMissionInst->status;
}

uint_t MissionInstanceIsWorking(uint_t id){
    if(MISSION_EXECUTING == g_cfg_mission[id].pMissionInst->status){
        return 1;
    }
    return 0;
}

uint_t MissionInstanceIsCompleted(uint_t id){
    if(MISSION_COMPLITED == g_cfg_mission[id].pMissionInst->status){
        return 1;
    }
    return 0;
}

uint_t MissionInstanceIsFailed(uint_t id){
    if(MISSION_COMPLITED == g_cfg_mission[id].pMissionInst->status && 0 != g_cfg_mission[id].pMissionInst->errorCode){
        return 1;
    }
    return 0;
}

uint_t MissionInstanceIsSucced(uint_t id){
    if(MISSION_COMPLITED == g_cfg_mission[id].pMissionInst->status && 0 == g_cfg_mission[id].pMissionInst->errorCode){
        return 1;
    }
    return 0;
}
//小于
uint_t MissionInstanceNodeEnteredCountLittle(uint_t id, uint_t node_id, int num){
    if( g_cfg_mission[id].pMissionInst->pNodeStatusArr[node_id].enteredCount < num ){
        return 1;
    }
    return 0;
}
//小于等于
uint_t MissionInstanceNodeEnteredCountLittleOrEqual(uint_t id, uint_t node_id, int num){
    if( g_cfg_mission[id].pMissionInst->pNodeStatusArr[node_id].enteredCount <= num ){
        return 1;
    }
    return 0;
}
//等于
uint_t MissionInstanceNodeEnteredCountEqual(uint_t id, uint_t node_id, int num){
    if( g_cfg_mission[id].pMissionInst->pNodeStatusArr[node_id].enteredCount == num ){
        return 1;
    }
    return 0;
}
//大于等于
uint_t MissionInstanceNodeEnteredCountBigOrEqual(uint_t id, uint_t node_id, int num){
    if( g_cfg_mission[id].pMissionInst->pNodeStatusArr[node_id].enteredCount >= num ){
        return 1;
    }
    return 0;
}
//大于
uint_t MissionInstanceNodeEnteredCountBig(uint_t id, uint_t node_id, int num){
    if( g_cfg_mission[id].pMissionInst->pNodeStatusArr[node_id].enteredCount > num ){
        return 1;
    }
    return 0;
}

void AllMissionsTick(void){
    uint_t i = 0;
    for(i = 0; i<G_MISSION_NUM; i++){
        if(MISSION_EXECUTING == g_cfg_mission[i].pMissionInst->status){
            MissionTick(g_cfg_mission[i].pMissionInst);
        }
    }
}

// 任务参数设置
void MissionInstanceSetArgs1(uint_t id, uint_t a1){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
}
void MissionInstanceSetArgs2(uint_t id, uint_t a1, uint_t a2){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
}
void MissionInstanceSetArgs3(uint_t id, uint_t a1, uint_t a2, uint_t a3){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
}
void MissionInstanceSetArgs4(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
}
void MissionInstanceSetArgs5(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
}
void MissionInstanceSetArgs6(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
}
void MissionInstanceSetArgs7(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
    g_cfg_mission[id].pMissionInst->args[6] = a7;
}
void MissionInstanceSetArgs8(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
    g_cfg_mission[id].pMissionInst->args[6] = a7;
    g_cfg_mission[id].pMissionInst->args[7] = a8;
}
void MissionInstanceSetArgs9(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
    g_cfg_mission[id].pMissionInst->args[6] = a7;
    g_cfg_mission[id].pMissionInst->args[7] = a8;
    g_cfg_mission[id].pMissionInst->args[8] = a9;
}
void MissionInstanceSetArgs10(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9, uint_t a10){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
    g_cfg_mission[id].pMissionInst->args[6] = a7;
    g_cfg_mission[id].pMissionInst->args[7] = a8;
    g_cfg_mission[id].pMissionInst->args[8] = a9;
    g_cfg_mission[id].pMissionInst->args[9] = a10;
}
void MissionInstanceSetArgs11(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9, uint_t a10, uint_t a11){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
    g_cfg_mission[id].pMissionInst->args[6] = a7;
    g_cfg_mission[id].pMissionInst->args[7] = a8;
    g_cfg_mission[id].pMissionInst->args[8] = a9;
    g_cfg_mission[id].pMissionInst->args[9] = a10;
    g_cfg_mission[id].pMissionInst->args[10] = a11;
}

void MissionInstanceSetArgs12(uint_t id, uint_t a1, uint_t a2, uint_t a3, uint_t a4, uint_t a5, uint_t a6, uint_t a7, uint_t a8, uint_t a9, uint_t a10, uint_t a11, uint_t a12 ){
    g_cfg_mission[id].pMissionInst->args[0] = a1;
    g_cfg_mission[id].pMissionInst->args[1] = a2;
    g_cfg_mission[id].pMissionInst->args[2] = a3;
    g_cfg_mission[id].pMissionInst->args[3] = a4;
    g_cfg_mission[id].pMissionInst->args[4] = a5;
    g_cfg_mission[id].pMissionInst->args[5] = a6;
    g_cfg_mission[id].pMissionInst->args[6] = a7;
    g_cfg_mission[id].pMissionInst->args[7] = a8;
    g_cfg_mission[id].pMissionInst->args[8] = a9;
    g_cfg_mission[id].pMissionInst->args[9] = a10;
    g_cfg_mission[id].pMissionInst->args[10] = a11;
    g_cfg_mission[id].pMissionInst->args[11] = a12;
}

// 发送任务消息
void MissionMessageArgs1(uint_t id, uchar_t arg1){
    MissionInstance* pInst = g_cfg_mission[id].pMissionInst;
    SendPacket(0xA5, 0x05, pInst->cmd_dst, pInst->cmd_src, pInst->command, &arg1, 1);
}
