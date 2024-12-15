/**********************************************************
filename   : all_missions.h
function   : 任务汇总
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#ifndef _ALL_MISSIONS_H_
#define _ALL_MISSIONS_H_

#include "common.h"
#include "public_missions.h"

/*=========================================================
 * 【配置】所有任务实例ID
*/
enum MISSION_IDS {
    MISSION_CLEAN_PLATE_ROTATE_INIT,                 // 旋转电机
    MISSION_CLEAN_PLATE_ROTATE_TO_X,                 // 
    MISSION_CLEAN_PLATE_ROTATE_TO_0,                 // 
    MISSION_CLEANER_SUCKER_VERT_INIT,                // 升降电机
    MISSION_CLEANER_SUCKER_VERT_TO_X,                // 
    MISSION_CLEANER_SUCKER_VERT_TO_0,                // 
    MISSION_SUBSTRATE_MIXER_VERT_INIT,               // 混匀升降电机
    MISSION_SUBSTRATE_MIXER_VERT_TO_X,               // 
    MISSION_SUBSTRATE_MIXER_VERT_TO_0,               // 
    MISSION_SUBSTRATE_A_PLUNGER_INIT,                // 柱塞泵电机
    MISSION_SUBSTRATE_A_PLUNGER_TO_X,                // 
    MISSION_SUBSTRATE_A_PLUNGER_TO_0,                // 
    MISSION_SUBSTRATE_B_PLUNGER_INIT,                // 柱塞泵电机
    MISSION_SUBSTRATE_B_PLUNGER_TO_X,                // 
    MISSION_SUBSTRATE_B_PLUNGER_TO_0,                // 
    MISSION_CLEANER_MIXER_VERT_INIT,                 // 混匀升降电机
    MISSION_CLEANER_MIXER_VERT_TO_X,                 // 
    MISSION_CLEANER_MIXER_VERT_TO_0,                 // 
    // 测试动作
    MISSION_DEMO_ACTION,                             // 示例动作
    // ENDING
    MISSION_UNKNOWN,
    MISSION_NUM
};
void InitAllMissions(void);
//=========================================================

#endif
