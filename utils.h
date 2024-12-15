/**********************************************************
filename   : utils.h
function   : 实用函数
author     : 谢令
created    : 2022-12-02
comment    :
**********************************************************/
#ifndef _UTILS_H_
#define _UTILS_H_

#include "common.h"

#define ENABLE_TRACE_MSG 1

// 发送报文
uint_t SendFrame(uchar_t* pPacket, uint_t packetSize);
uint_t SendPacket(uchar_t ctrl, uchar_t len, uchar_t src, uchar_t dst, ushort_t cmd, uchar_t* pParams, uint_t paramsLen);
uint_t SendErrorResponsePacket(uchar_t *pPacket, ushort_t err_cmd_code);

uint_t SendMissionTraceMsg(uchar_t miss_id, uchar_t node_id);
#ifdef ENABLE_TRACE_MSG
    #define SEND_MISSION_TRACE_MSG(miss_id, node_id)  SendMissionTraceMsg((miss_id), (node_id));
#else 
    #define SEND_MISSION_TRACE_MSG(miss_id, node_id)
#endif

#endif
