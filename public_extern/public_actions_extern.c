/**********************************************************
filename   : public_actions_extern.c
function   : 动作执行接口（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#include "public_missions.h"
#include "public_actions_extern.h"

#include "sensor_counter_processors.h"

// 执行动作
int ActionExternExecute(MissionInstance* pMission, Action* pAct){
    switch(pAct->type){
        default:{
            break;
        }
    }
    return 0;
}
