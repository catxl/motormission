/**********************************************************
filename   : utils.c
function   : 实用函数
author     : 谢令
created    : 2022-12-02
comment    :
**********************************************************/

#include "utils.h"
#include "byte_sequence.h"
#include "can_io.h"
#include "commpacket.h"
#include "protocol.h"
#include "usart_io.h"

// 发送数据
uint_t SendFrame(uchar_t* pPacket, uint_t packetSize) {
    uint_t sendlen = 0;
    sendlen = can_io_send(pPacket, packetSize);
    return sendlen;
}

uint_t SendPacket(uchar_t ctrl, uchar_t len, uchar_t src, uchar_t dst, ushort_t cmd, uchar_t* pParams, uint_t paramsLen) {
    uchar_t packetBuf[30] = {0};
    uint_t packetLen = FillCommPacket(packetBuf, 30, ctrl, len, src, dst, cmd, pParams, paramsLen);
    return SendFrame(packetBuf, packetLen);
}

uint_t SendErrorResponsePacket(uchar_t* pPacket, ushort_t err_cmd_code) {
    uchar_t err[2] = {0};
    ushort_to_big_endian(err_cmd_code, err);
    return SendPacket(0xF5, 0x06, GetPackFieldDst(pPacket), GetPackFieldSrc(pPacket), GetPackFieldCmd(pPacket), err, 2);
}

uint_t SendMissionTraceMsg(uchar_t miss_id, uchar_t node_id){
    uchar_t msg[2] = {0};
    msg[0] = miss_id;
    msg[1] = node_id;
    return SendPacket(0x00, 0x06, MYID, SYS_U_PC, 0x9601, msg, 2);
}
