/**********************************************************
filename   : step_motors.c
function   : 步进电机
author     : 谢令
created    : 2022-11-03
comment    :
**********************************************************/
#include "step_motors.h"

//=================================================================
/*
 * 本单元使用到的电机的实现定义
*/
ushort_t g_moter_speed_reg_addr[MOTOR_NUMBER] = {          // 配置各个电机的速度寄存器地址
    0xF000, 0xF100, 0xF200, 0xF300, 0xF400, 0xF500, 0xF600, 0xF700, 0xF800, 0xF900, 0xFA00, 0xFB00, 0xFC00, 0xFD00, 0xFE00
};
ushort_t g_moter_step_reg_addr[MOTOR_NUMBER] = {           // 配置各个电机的步数寄存器地址
    0xF001, 0xF101, 0xF201, 0xF301, 0xF401, 0xF501, 0xF601, 0xF701, 0xF801, 0xF901, 0xFA01, 0xFB01, 0xFC01, 0xFD01, 0xFE01
};
MOTOR_RUNTIME_STATUS g_moter_runtime_status[MOTOR_NUMBER] = {
    {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE},
    {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE},
    {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}, {STEP_MOTOR_AVAILABLE}
};

//配置电机前进和后退的方向
STEP_MOTOR_CONF   g_cfg_step_motor[MOTOR_NUMBER] = {
    //motor_id                                      //[0]后退 //[1]前进(远离光耦方向)     
    {STEP_MOTOR_CLEAN_PLATE_ROTATE_M0,              {0, 1} },// 
    {STEP_MOTOR_CLEANER_SUCKER_VERT_M1,             {1, 0} },// 
    {STEP_MOTOR_SUBSTRATE_MIXER_VERT_M2,            {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M01,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M02,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M03,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M04,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M05,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M06,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M07,  {0, 1} },// 
    {STEP_MOTOR_SPIT_LIQUID_PERSISTALTIC_PUMP_M08,  {0, 1} },// 
    {STEP_MOTOR_SUBSTRATE_A_PLUNGER_M11,            {1, 0} },// 
    {STEP_MOTOR_SUBSTRATE_B_PLUNGER_M12,            {1, 0} },// 
    {STEP_MOTOR_CLEANER_MIXER_VERT_M13,             {1, 0} },// 
    {STEP_MOTOR_SUCK_LIQUID_PERSISTALTIC_PUMP_M14,  {0, 1} },// 
};
