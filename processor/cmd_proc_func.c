/**********************************************************
filename   : cmd_proc_func.c
function   : 命令处理函数
author     : 谢令
created    : 2022-11-21
comment    :
**********************************************************/
#include "cmd_proc_func.h"
#include "all_missions.h"

// 0x0001 执行Demo
// 68 25 04 00 02 00 01 ff 16 -> 68 a5 04 02 00 00 01 zz 16
int CmdProcExecDemo(uchar_t* pPacket) {
    MissionInstanceStart(pPacket, MISSION_DEMO_ACTION);
    return 0;
}
