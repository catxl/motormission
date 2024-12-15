/**********************************************************
filename   : public_indicators.c
function   : 指示器接口定义（公共部分）
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#include "public_missions.h"
#include "public_indicators_extern.h"
#include "public_detectors.h"

#include "public_photosensors.h"
#include "public_timers.h"
#include "public_common_motors.h"

/*
 * 指示器输出指引: 判断指定的条件是否触发
 *          返回：0(未触发)1(已触发)
*/
int IndicatorShow(MissionInstance* pMission, Indicator* pIndctArr, int id){
    switch(pIndctArr[id].type){
        case INDCT_ALWAYS_ACTIVE:{       // 始终都是触发状态
            return 1;
        }
        case INDCT_ALWAYS_NOT_ACTIVE:{   // 永不触发
            return 0;
        }
        case INDCT_MOTOR_STOPED:{        // 电机已停止
            uint_t motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(0 == step_motor_get_remain_step(motor_id)){
                return 1;
            }
            return 0;
        }
        case INDCT_MOTOR_GOTIT:{         // 电机获取成功
            uint_t motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(step_motor_try_lock(motor_id)){
                return 1;
            }
            return 0;
        }
        case INDCT_MOTOR_GOTIT_IF_STOPED:{// 电机停止后获取成功
            uint_t motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(0 == step_motor_get_remain_step(motor_id) && step_motor_try_lock(motor_id)){
                return 1;
            }
            return 0;
        }
        case INDCT_COMMON_MOTOR_GOTIT_IF_STOPED:{// 电机停止后获取成功
            uint_t common_motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(0 == common_motor_get_remain_step(common_motor_id) && common_motor_try_lock(common_motor_id)){
                return 1;
            }
            return 0;
        }
        case INDCT_MOTOR_REMAIN_STEP_LE:{// 电机移动剩余步数小于等于(Less or Equal)
            uint_t motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            ushort_t expct_val = (ushort_t) MissionGetFunctorArg(pMission, pIndctArr, 1);
            if(step_motor_get_remain_step(motor_id) <= expct_val){
                return 1;
            }
            break;
        }
        case INDCT_COMMON_MOTOR_REMAIN_STEP_LE:{// 电机移动剩余步数小于等于(Less or Equal)
            uint_t common_motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            ushort_t expct_val = (ushort_t) MissionGetFunctorArg(pMission, pIndctArr, 1);
            if(common_motor_get_remain_step(common_motor_id) <= expct_val){
                return 1;
            }
            break;
        }
        case INDCT_COMMON_MOTOR_STOPPED:{// 普通电机停止运行
            uint_t common_motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(common_motor_is_stopped(common_motor_id)){
                return 1;
            }
            break;
        }
        case INDCT_COMMON_MOTOR_COMPENSATE_DONE:{ // 普通电机补偿完成
            uint_t common_motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(common_motor_is_compensate_done(common_motor_id)){
                return 1;
            }
            break;
        }
        case INDCT_SENSOR_ACTIVE:{       // 传感器激活
            uint_t sensor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(PHOTOSENSOR_ACTIVED == GetPhotoSensorStatus(sensor_id)){
                return 1;
            }
            break;
        }
        case INDCT_SENSOR_NOT_ACTIVE:{   // 传感器未激活
            uint_t sensor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(PHOTOSENSOR_NOT_ACTIVED == GetPhotoSensorStatus(sensor_id)){
                return 1;
            }
            break;
        }
        case INDCT_TIMEOUT:{             // 计时器超时
            uint_t timer_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(IsTimeOut(timer_id)){
                return 1;
            }
            break;
        }
        case INDCT_REG_VALUE_EQ:{        // 寄存器A的值与X相等
            uint_t reg_addr = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            ushort_t expct_val = (ushort_t) MissionGetFunctorArg(pMission, pIndctArr, 1);
            if(expct_val == FPGA(reg_addr)){
                return 1;
            }
            break;
        }
        case INDCT_MISSION_COMPLETED:{   // 任务已完成
            uint_t mission_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            return MissionInstanceIsCompleted(mission_id);
        }
        case INDCT_MISSION_FAIL: { // 任务失败
            uint_t mission_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            return MissionInstanceIsFailed(mission_id);
        }
        case INDCT_MISSION_NODE_ENTERED_CNT_LE: {  // 任务节点进入次数小于等于X
            uint_t node_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            uint_t num = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 1);
            return MissionInstanceNodeEnteredCountLittleOrEqual(pMission->my_id, node_id, num);
        }
        case INDCT_MISSION_NODE_ENTERED_CNT_EQUAL: {  // 任务节点进入次数等于X
            uint_t node_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            uint_t num = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 1);
            return MissionInstanceNodeEnteredCountEqual(pMission->my_id, node_id, num);
        }
        case INDCT_MISSION_NODE_ENTERED_CNT_PASS: {  // 任务节点进入次数大于X
            uint_t node_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            uint_t num = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 1);
            return MissionInstanceNodeEnteredCountBig(pMission->my_id, node_id, num);
        }
        case INDCT_MISSION_NODE_ENTERED_CNT_LESS: {  // 任务节点进入次数小于X
            uint_t node_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            uint_t num = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 1);
            return MissionInstanceNodeEnteredCountLittle(pMission->my_id, node_id, num);
        }
        case INDCT_MISSION_NODE_ENTERED_CNT_GE: {    // 任务节点进入次数大于等于X
            uint_t node_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            uint_t num = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 1);
            return MissionInstanceNodeEnteredCountBigOrEqual(pMission->my_id, node_id, num);
        }
        case INDCT_COMMON_MOTOR_ACT_SUCC:{  // 普通电机动作已完成
            uint_t common_motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            return common_motor_is_move_succ(common_motor_id);
        }
        case INDCT_COMMON_MOTOR_ACT_FAIL:{  // 普通电机动作失败
            uint_t common_motor_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            return common_motor_is_move_fail(common_motor_id);
        }
        case INDCT_DETECTOR_TRIGGERED:{   // 探测器已触发
            uint_t detector_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            return IsDetectorTriggered(detector_id);
        }
        case INDCT_DETECTOR_NOT_TRIGGERED:{   // 探测器未触发
            uint_t detector_id = MissionGetFunctorArg(pMission, &(pIndctArr[id]), 0);
            if(IsDetectorTriggered(detector_id)){
                return 0;
            }
            return 1;
        }
        case INDCT_LOGIC_AND:{           // 多个指示器'与'(AND)
            uint_t indct_num = pIndctArr[id].args[0];
            uint_t i = 0;
            for(i=0; i<indct_num; i++){
                if(!IndicatorShow(pMission, pIndctArr, pIndctArr[id].args[1+i])){
                    return 0;
                }
            }
            return 1;
        }
        case INDCT_LOGIC_OR:{            // 多个指示器'或'(OR)
            uint_t indct_num = pIndctArr[id].args[0];
            uint_t i = 0;
            for(i=0; i<indct_num; i++){
                if(IndicatorShow(pMission, pIndctArr, pIndctArr[id].args[1+i])){
                    return 1;
                }
            }
            return 0;
        }
        case INDCT_LOGIC_NOT:{            // 多个指示器'非'(NOT)
            uint_t indct_num = pIndctArr[id].args[0];
            uint_t i = 0;
            for(i=0; i<indct_num; i++){
                if(IndicatorShow(pMission, pIndctArr, pIndctArr[id].args[1+i])){
                    return 0;
                }
            }
            return 1;
        }
        case INDCT_DEBUG_TRACKING:{       // 调试跟踪
            return 1;
        }
        default:{
            return IndicatorExternShow(pMission, pIndctArr, id);
        }
    }
    return 0;
}


/*
 * 指示器刷新: 重新计算所有指示器
*/
int IndicatorsRefresh(MissionInstance * pMission, Indicator* pIndctArr, uint_t num){
    uint_t changed_flag = 0;
    do{
        int i = 0;
        changed_flag = 0;
        for(i=0; i<num; i++){
            if(IndicatorShow(pMission, pIndctArr, i)){
                changed_flag = 1;
            }
        }
    }while(changed_flag);
    return 0;
}
