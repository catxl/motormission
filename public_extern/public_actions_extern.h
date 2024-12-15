/**********************************************************
filename   : public_actions_extern.h
function   : 动作执行接口（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#ifndef _PUBLIC_ACTIONS_EXTERN_H_
#define _PUBLIC_ACTIONS_EXTERN_H_

#include "common.h"
#include "public_actions.h"

/*
 * 动作类型
*/
enum ACTION_EXTERN_TYPE {
    ACTION_EXTERN_TYPE_BEGIN = ACTION_TYPE_NUM - 1,  // <--!!这行不能删
    // TODO: 添加扩展类型
    ACTION_EXTERN_TYPE_NUM
};

// 执行动作
int ActionExternExecute(MissionInstance * pMission, Action* pActArr);

#endif
