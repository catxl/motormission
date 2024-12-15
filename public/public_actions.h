/**********************************************************
filename   : public_actions.h
function   : 动作执行接口（公共部分）
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#ifndef _PUBLIC_ACTIONS_H_
#define _PUBLIC_ACTIONS_H_

#include "common.h"
#include "public_missions.h"

/*
 * 动作类型
*/
enum ACTION_TYPE {
    ACT_NO_OPERATION,                    // 无操作
    ACT_VALVE_TRIGGER,                   // 伐操作
    ACT_LED_TRIGGER,                     // LED操作
    ACT_SWITCH_TRIGGER,                  // 开关操作
    ACT_TIMEOUT_START,                   // 启动倒计时
    ACT_TIMER_STOP,                      // 停止计时器
    ACT_MOTOR_MOVE,                      // 步进电机转动
    ACT_MOTOR_MOVE_REVERSE,              // 步进电机反向转动
    ACT_MOTOR_RELEASE,                   // 步进释放占用
    ACT_DC_MOTOR_MIXING_START,           // 直流电机开始混匀转动
    ACT_DC_MOTOR_MIXING_STOP,            // 直流电机停止混匀转动
    ACT_COMMON_MOTOR_RELEASE,            // 普通电机释放占用
    ACT_COMMON_MOTOR_MOVE_STEP_X,        // 普通电机运行第X步骤(Step_X)
    ACT_COMMON_MOTOR_MV_COMPENSATE,      // 普通电机运行补偿
    ACT_COMMON_MOTOR_MOVE,               // 普通电机运动到X位置
    ACT_COMMON_MOTOR_MOVE_V3,            // 普通电机运动到X位置-3个参数：dir-speed-dest
    ACT_COMMON_MOTOR_DONE,               // 普通电机完成
    ACT_DETECTOR_START,                  // 探测器启动
    ACT_DETECTOR_STOP,                   // 探测器停止
    ACT_MISSION_START,                   // 启动任务
    ACT_MISSION_START_ARG1,              // 启动任务: 带 1 个参数
    ACT_MISSION_START_ARG2,              // 启动任务: 带 2 个参数
    ACT_MISSION_START_ARG3,              // 启动任务: 带 3 个参数
    ACT_MISSION_START_ARG4,              // 启动任务: 带 4 个参数
    ACT_MISSION_START_ARG5,              // 启动任务: 带 5 个参数
    ACT_MISSION_DONE,                    // 任务完成
    ACT_MISSION_FAIL,                    // 任务失败
    ACT_MISSION_MESSAGE_ARG1,            // 任务消息: 带 1 个参数
    ACT_SELFDEF_FUNC,                    // 执行自定义函数
    ACT_DEBUG_TRACKING,                  // 用于调试跟踪的动作
    ACTION_TYPE_NUM
};

// 定义任务动作执行函数指针类型
typedef void (*MISSION_ACTION_FUNC)(MissionInstance * pMission);
// 执行动作
int ActionExecute(MissionInstance * pMission, Action* pActArr);
// 批量执行动作
int ActionBatchExecute(MissionInstance * pMission, Action* pActArr, uint_t num);

#endif
