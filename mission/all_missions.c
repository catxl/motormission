/**********************************************************
filename   : all_missions.c
function   : 任务汇总
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#include "all_missions.h"

/*=========================================================
 * 【配置】所有任务实例声明
*/
extern MissionInstance g_mission_inst_common_motor_init[];
extern MissionInstance g_mission_inst_common_motor_move[];
extern MissionInstance g_mission_inst_demo_action[];
//=========================================================

const int G_MISSION_NUM = MISSION_NUM;
MissionConf g_cfg_mission[MISSION_NUM] = {
    {MISSION_CLEAN_PLATE_ROTATE_INIT,         &g_mission_inst_common_motor_init[0]},// 盘旋转电机
    {MISSION_CLEAN_PLATE_ROTATE_TO_X,         &g_mission_inst_common_motor_move[0]},// 
    {MISSION_CLEAN_PLATE_ROTATE_TO_0,         &g_mission_inst_common_motor_move[0]},// 
    {MISSION_CLEANER_SUCKER_VERT_INIT,        &g_mission_inst_common_motor_init[1]},// 升降电机
    {MISSION_CLEANER_SUCKER_VERT_TO_X,        &g_mission_inst_common_motor_move[1]},// 
    {MISSION_CLEANER_SUCKER_VERT_TO_0,        &g_mission_inst_common_motor_move[1]},// 
    {MISSION_SUBSTRATE_MIXER_VERT_INIT,       &g_mission_inst_common_motor_init[2]},// 混匀升降电机
    {MISSION_SUBSTRATE_MIXER_VERT_TO_X,       &g_mission_inst_common_motor_move[2]},// 
    {MISSION_SUBSTRATE_MIXER_VERT_TO_0,       &g_mission_inst_common_motor_move[2]},// 
    {MISSION_SUBSTRATE_A_PLUNGER_INIT,        &g_mission_inst_common_motor_init[3]},// 柱塞泵电机
    {MISSION_SUBSTRATE_A_PLUNGER_TO_X,        &g_mission_inst_common_motor_move[3]},// 
    {MISSION_SUBSTRATE_A_PLUNGER_TO_0,        &g_mission_inst_common_motor_move[3]},// 
    {MISSION_SUBSTRATE_B_PLUNGER_INIT,        &g_mission_inst_common_motor_init[4]},// 柱塞泵电机
    {MISSION_SUBSTRATE_B_PLUNGER_TO_X,        &g_mission_inst_common_motor_move[4]},// 
    {MISSION_SUBSTRATE_B_PLUNGER_TO_0,        &g_mission_inst_common_motor_move[4]},// 
    {MISSION_CLEANER_MIXER_VERT_INIT,         &g_mission_inst_common_motor_init[5]},// 混匀升降电机
    {MISSION_CLEANER_MIXER_VERT_TO_X,         &g_mission_inst_common_motor_move[5]},// 
    {MISSION_CLEANER_MIXER_VERT_TO_0,         &g_mission_inst_common_motor_move[5]},// 
    // 测试动作
    {MISSION_DEMO_ACTION,                     &g_mission_inst_demo_action[0]} // 示例动作
    
};

// 初始化所有任务参数：通过配置实现的任务可在此初始化
void InitAllMissions(void){

}
