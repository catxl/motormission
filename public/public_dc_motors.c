/**********************************************************
filename   : public_dc_motors.c
function   : 直流电机接口定义（公共部分）
author     : 谢令
created    : 2022-11-17
comment    :
**********************************************************/
#include "public_dc_motors.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
 */
extern const uint_t G_DC_MOTORS_NUMBER;    // 配置直流混匀电机数量
extern DC_MOTOR_CONFIG g_cfg_dc_motor[];   // 配置直流混匀电机基础信息
extern DC_MOTOR_STATUS g_st_dc_motor[];    // 存储直流混匀电机状态的内存
//=================================================================

/*
 * 启动、停止直流电机混匀动作
 */
ushort_t StartDcMotor(ushort_t id) {
    FPGA(g_cfg_dc_motor[id].uiRegBaseAddr) = g_cfg_dc_motor[id].uiSpeedLevel;  // 开始混匀
    g_st_dc_motor[id].ucCurrentStatus = DC_MOTOR_WORKING;
    return 0;
}

ushort_t StartDcMotorWithSpeed(ushort_t id, ushort_t speed) {
    FPGA(g_cfg_dc_motor[id].uiRegBaseAddr) = speed;
    if (0 != speed) {
        g_st_dc_motor[id].ucCurrentStatus = DC_MOTOR_WORKING;
    }
    else {
        g_st_dc_motor[id].ucCurrentStatus = DC_MOTOR_FREE;
    }
    return 0;
}

ushort_t StopDcMotor(ushort_t id) {
    FPGA(g_cfg_dc_motor[id].uiRegBaseAddr) = 0;
    g_st_dc_motor[id].ucCurrentStatus = DC_MOTOR_FREE;
    return 0;
}
