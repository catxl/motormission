/**********************************************************
filename   : comm_proc.c
function   : 通信程序协议解释和处理程序
author     : 谢令
created    : 2022-11-22
comment    :
**********************************************************/
#include "comm_proc.h"
#include "utils.h"
// workers
#include "valves.h"
#include "step_motors.h"
#include "dc_motors.h"
#include "photosensors.h"
// io
#include "commpacket.h"
#include "comm_buffer.h"
#include "eeprom.h"
#include "can_io.h"
#include "usart_io.h"
#include "protocol.h"
// command process
#include "cmd_proc_func.h"
#include "errcode.h"

void PacketProcess(uchar_t *pPack)
{
    ushort_t cmd = GetPackFieldCmd(pPack);
    uint_t i = 0;
    for (i = 0; i < COMM_CMD_NUMBER; i++)
    {
        if (cmd == g_cfg_comm_cmd[i].cmdCode)
        {
            if(g_cfg_comm_cmd[i].isFixedLen && 
               g_cfg_comm_cmd[i].paramsLenth != GetPackFieldParamsLen(pPack))
            {
                SendPacket(GetPackFieldCtrl(pPack)|0xC0, 0x04, 
                           GetPackFieldDst(pPack), GetPackFieldSrc(pPack), ERR_CMD_DATA_LENGTH, 0, 0);
                return;
            }
            if (g_cfg_comm_cmd[i].procFunc)
            {
                g_cfg_comm_cmd[i].procFunc(pPack);
            }
            break;
        }
    }
}

void CommPackProcess(void)
{
    uchar_t frameBuf[MAX_FRAME_LEN] = {0};
    uint_t frameLen = FillCommPacketFromCommBuffer(frameBuf, MAX_FRAME_LEN, CAN_RECV_BUFFER);
    if (0 == frameLen)
    {
        frameLen = FillCommPacketFromCommBuffer(frameBuf, MAX_FRAME_LEN, USART3_RECV_BUFFER);
    }
    if (0 == frameLen)
    {
        return;
    }
    if (MYID == GetPackFieldDst(frameBuf))
    {
        PacketProcess(frameBuf);
    }
}
