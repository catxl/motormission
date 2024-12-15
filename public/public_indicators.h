/**********************************************************
filename   : public_indicators.h
function   : 指示器接口定义（公共部分）
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#ifndef _PUBLIC_INDICATORS_H_
#define _PUBLIC_INDICATORS_H_

#include "common.h"

/*
 * 指示器类型
*/
enum INDICATOR_TYPE {
    INDCT_ALWAYS_ACTIVE,
    INDCT_ALWAYS_NOT_ACTIVE,
    INDCT_MOTOR_STOPED,                    // 电机已停止
    INDCT_MOTOR_GOTIT,                     // 电机获取成功
    INDCT_MOTOR_GOTIT_IF_STOPED,           // 电机停止后获取成功
    INDCT_COMMON_MOTOR_GOTIT_IF_STOPED,    // 普通电机停止后获取成功
    INDCT_MOTOR_REMAIN_STEP_LE,            // 电机移动剩余步数小于等于(Less or Equal)
    INDCT_COMMON_MOTOR_REMAIN_STEP_LE,     // 普通电机移动剩余步数小于等于(Less or Equal)
    INDCT_COMMON_MOTOR_STOPPED,            // 普通电机停止转动
    INDCT_COMMON_MOTOR_COMPENSATE_DONE,    // 普通电机补偿完成
    INDCT_SENSOR_ACTIVE,                   // 传感器激活
    INDCT_SENSOR_NOT_ACTIVE,               // 传感器未激活
    INDCT_TIMEOUT,                         // 计时器超时
    INDCT_REG_VALUE_EQ,                    // 寄存器A的值与X相等
    INDCT_MISSION_COMPLETED,               // 任务已完成
    INDCT_MISSION_FAIL,                    // 任务失败
    INDCT_MISSION_NODE_ENTERED_CNT_LE,     // 任务节点进入次数小于等于X
    INDCT_MISSION_NODE_ENTERED_CNT_EQUAL,  // 任务节点进入次数等于X
    INDCT_MISSION_NODE_ENTERED_CNT_PASS,   // 任务节点进入次数大于X
    INDCT_MISSION_NODE_ENTERED_CNT_LESS,   // 任务节点进入次数小于X
    INDCT_MISSION_NODE_ENTERED_CNT_GE,     // 任务节点进入次数大于等于X
    INDCT_COMMON_MOTOR_ACT_SUCC,           // 普通电机动作已完成
    INDCT_COMMON_MOTOR_ACT_FAIL,           // 普通电机动作失败
    INDCT_DETECTOR_TRIGGERED,              // 探测器已触发
    INDCT_DETECTOR_NOT_TRIGGERED,          // 探测器未触发
    INDCT_LOGIC_AND,                       // 多个指示器'与'(AND)
    INDCT_LOGIC_OR,                        // 多个指示器'或'(OR)
    INDCT_LOGIC_NOT,                       // 多个指示器'非'(NOT)
    INDCT_DEBUG_TRACKING,                  // 用于调试跟踪的指示器
    INDICATOR_TYPE_NUM
};

/*
 * 指示器输出指引: 判断指定的条件是否触发
 *          返回：0(未触发)1(已触发)
*/
int IndicatorShow(MissionInstance * pMission, Indicator* pIndctArr, int id);
/*
 * 指示器刷新: 重新计算所有指示器
*/
int IndicatorsRefresh(MissionInstance * pMission, Indicator* pIndctArr, uint_t num);

#endif
