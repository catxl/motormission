/**********************************************************
filename   : public_actions.c
function   : 动作执行接口（公共部分）
author     : 谢令
created    : 2022-11-27
comment    :
**********************************************************/
#include "public_missions.h"
#include "public_actions_extern.h"

#include "public_valves.h"
#include "public_step_motors.h"
#include "public_dc_motors.h"
#include "public_timers.h"
#include "public_switches.h"
#include "public_common_motors.h"
#include "public_detectors.h"

// 执行动作
int ActionExecute(MissionInstance* pMission, Action* pAct){
    switch(pAct->type){
        case ACT_NO_OPERATION:{             // 无操作
            break;
        }
        case ACT_VALVE_TRIGGER:{            // 伐操作，参数序号: 0(on/off)，1(阀的数量)，2-5阀的ID
            uint_t on_off = MissionGetFunctorArg(pMission, pAct, 0);
            uint_t valves_num = MissionGetFunctorArg(pMission, pAct, 1);
            ushort_t valves_ids[4] = {0};
            uint_t i = 0;
            for(i=0; i<valves_num; i++){
                valves_ids[i] = MissionGetFunctorArg(pMission, pAct, 2+i);
            }
            if(VALVE_CLOSED==on_off){
                ValveBatchTurnOff(valves_ids, valves_num);
            }
            else{
                ValveBatchTurnOn(valves_ids, valves_num);
            }
            break;
        }
        case ACT_LED_TRIGGER:{              // LED操作
             break;
        }
        case ACT_SWITCH_TRIGGER:{           // 开关操作，参数序号: 0(on/off)，1(开关的ID)
            uint_t on_off = MissionGetFunctorArg(pMission, pAct, 0);
            ushort_t switch_id = MissionGetFunctorArg(pMission, pAct, 1);
            SwitchSet(switch_id, on_off);
            break;
        }
        case ACT_TIMEOUT_START:{            // 启动倒计时，参数序号: 0(计时器的ID)，1(超时ms)
            TimerStartCountDown(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1));
            break;
        }
        case ACT_TIMER_STOP:{               // 停止计时器，参数序号: 0(计时器的ID)
            TimerStop(MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_MOTOR_MOVE:{               // 步进电机转动，参数序号: 0(步进电机的ID)，1(方向)，2(速度)，3(步数)
            step_motor_move(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1), 
                            MissionGetFunctorArg(pMission, pAct, 2), MissionGetFunctorArg(pMission, pAct, 3));
            break;
        }
        case ACT_MOTOR_MOVE_REVERSE:{       // 步进电机反向转动，参数序号: 0(步进电机的ID)，1(方向)，2(速度)，3(步数)
            step_motor_move_reverse(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1), 
                            MissionGetFunctorArg(pMission, pAct, 2), MissionGetFunctorArg(pMission, pAct, 3));
            break;
        }
        case ACT_MOTOR_RELEASE:{            // 步进释放占用
            step_motor_unlock(MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_DC_MOTOR_MIXING_START:{    // 直流电机开始混匀转动，参数序号: 0(直流电机ID)
            StartDcMotor(MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_DC_MOTOR_MIXING_STOP:{     // 直流电机停止混匀转动，参数序号: 0(直流电机ID)
            StopDcMotor(MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_COMMON_MOTOR_RELEASE:{     // 普通电机释放占用，参数序号: 0(普通电机ID)
            common_motor_unlock(MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_COMMON_MOTOR_MOVE_STEP_X:{ // 普通电机运行第X步骤，参数序号: 0(普通电机ID), 1(动作类型), 1(第X步)
            common_motor_move_step_x(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1), MissionGetFunctorArg(pMission, pAct, 2));
            break;
        }
        case ACT_COMMON_MOTOR_MV_COMPENSATE:{
            common_motor_start_compensate(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1));
            break;
        }
        case ACT_COMMON_MOTOR_MOVE:{        // 普通电机运动到X位置，参数序号: 0(普通电机ID), 1(动作类型)，2(目标位置)
            common_motor_move(0, MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1), MissionGetFunctorArg(pMission, pAct, 2));
            break;
        }
        case ACT_COMMON_MOTOR_MOVE_V3:{     // 普通电机运动到X位置-3个参数：dir-speed-dest
            common_motor_move3(0, MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1), 
                                  MissionGetFunctorArg(pMission, pAct, 2), MissionGetFunctorArg(pMission, pAct, 3), 
                                  MissionGetFunctorArg(pMission, pAct, 4));
            break;
        }
        case ACT_COMMON_MOTOR_DONE:{        // 普通电机完成，参数序号: 0(普通电机ID)，1(动作类型)，2(错误码)
            common_motor_move_finished(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1), MissionGetFunctorArg(pMission, pAct, 2));
            break;
        }
        case ACT_DETECTOR_START:{           // 探测器启动，参数序号: 0(探测器ID)，1(参数1)
            DetectorStart(MissionGetFunctorArg(pMission, pAct, 0), MissionGetFunctorArg(pMission, pAct, 1));
            break;
        }
        case ACT_DETECTOR_STOP:{            // 探测器停止，参数序号: 0(探测器ID)
            DetectorStop(MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_MISSION_START:{            // 启动任务，参数序号: 0(任务ID)
            uint_t mission_id = MissionGetFunctorArg(pMission, pAct, 0);
            MissionInstanceStart(0, mission_id);
            break;
        }
        case ACT_MISSION_START_ARG1:{        // 启动任务，参数序号: 0(任务ID)1(参数1)
            uint_t mission_id = MissionGetFunctorArg(pMission, pAct, 0);
            uint_t arg1 = MissionGetFunctorArg(pMission, pAct, 1);
            MissionInstanceSetArgs1(mission_id, arg1);
            MissionInstanceStart(0, mission_id);
            break;
        }
        case ACT_MISSION_START_ARG2:{        // 启动任务，参数序号: 0(任务ID)1(参数1)……
            uint_t mission_id = MissionGetFunctorArg(pMission, pAct, 0);
            uint_t arg1 = MissionGetFunctorArg(pMission, pAct, 1);
            uint_t arg2 = MissionGetFunctorArg(pMission, pAct, 2);
            MissionInstanceSetArgs2(mission_id, arg1, arg2);
            MissionInstanceStart(0, mission_id);
            break;
        }
        case ACT_MISSION_START_ARG3:{        // 启动任务，参数序号: 0(任务ID)1(参数1)……
            uint_t mission_id = MissionGetFunctorArg(pMission, pAct, 0);
            uint_t arg1 = MissionGetFunctorArg(pMission, pAct, 1);
            uint_t arg2 = MissionGetFunctorArg(pMission, pAct, 2);
            uint_t arg3 = MissionGetFunctorArg(pMission, pAct, 3);
            MissionInstanceSetArgs3(mission_id, arg1, arg2, arg3);
            MissionInstanceStart(0, mission_id);
            break;
        }
        case ACT_MISSION_START_ARG4:{        // 启动任务，参数序号: 0(任务ID)1(参数1)……
            uint_t mission_id = MissionGetFunctorArg(pMission, pAct, 0);
            uint_t arg1 = MissionGetFunctorArg(pMission, pAct, 1);
            uint_t arg2 = MissionGetFunctorArg(pMission, pAct, 2);
            uint_t arg3 = MissionGetFunctorArg(pMission, pAct, 3);
            uint_t arg4 = MissionGetFunctorArg(pMission, pAct, 4);
            MissionInstanceSetArgs4(mission_id, arg1, arg2, arg3, arg4);
            MissionInstanceStart(0, mission_id);
            break;
        }
        case ACT_MISSION_START_ARG5:{        // 启动任务，参数序号: 0(任务ID)1(参数1)……
            uint_t mission_id = MissionGetFunctorArg(pMission, pAct, 0);
            uint_t arg1 = MissionGetFunctorArg(pMission, pAct, 1);
            uint_t arg2 = MissionGetFunctorArg(pMission, pAct, 2);
            uint_t arg3 = MissionGetFunctorArg(pMission, pAct, 3);
            uint_t arg4 = MissionGetFunctorArg(pMission, pAct, 4);
            uint_t arg5 = MissionGetFunctorArg(pMission, pAct, 5);
            MissionInstanceSetArgs5(mission_id, arg1, arg2, arg3, arg4, arg5);
            MissionInstanceStart(0, mission_id);
            break;
        }
        case ACT_MISSION_DONE:              // 任务完成，参数序号: 0(错误码)
        case ACT_MISSION_FAIL:{             // 任务失败，参数序号: 0(错误码)
            MissionInstanceDone(pMission->my_id, MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_MISSION_MESSAGE_ARG1:{     // 任务消息: 带 1 个参数
            MissionMessageArgs1(pMission->my_id, MissionGetFunctorArg(pMission, pAct, 0));
            break;
        }
        case ACT_SELFDEF_FUNC:{             // 自定义执行函数，参数序号: 0(函数入口地址)
            ((MISSION_ACTION_FUNC)MissionGetFunctorArg(pMission, pAct, 0))(pMission);
            break;
        }
        case ACT_DEBUG_TRACKING:{           // 用于调试跟踪的动作
            break;
        }
        default:{
            ActionExternExecute(pMission, pAct);
            break;
        }
    }
    return 0;
}


// 批量执行动作
int ActionBatchExecute(MissionInstance * pMission, Action* pActArr, uint_t num){
    uint_t i = 0;
    for(i=0; i<num; i++){
        ActionExecute(pMission, &(pActArr[i]));
    }
    return 0;
}
