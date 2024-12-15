/**********************************************************
filename   : commpacket.h
function   : 通信包
author     : 谢令
created    : 2022-11-20
comment    :
**********************************************************/
#ifndef _IO_COMM_PACKET_H_
#define _IO_COMM_PACKET_H_

#include "common.h"

// 判断pBuff指向的内存是否是完整有效的包，如果是返回其长度，否则返回0, skipsz返回无效的数据的数量
uint_t IsValidPacket(uchar_t* pBuff, uint_t sz, uint_t *skipsz);
// 判断CommBuffer中是否有完整有效的包，如果是返回其长度，否则返回0, skipsz返回无效的数据的数量
uint_t IsValidPacketFromCommBuffer(uint_t commBuffId, uint_t *skipsz);
// 向 pPackBuff 填充一个包，返回实际填充的字节数
uint_t FillCommPacket(uchar_t* pPackBuff, uint_t buffSz, uchar_t ctrl, uchar_t len, uchar_t src, uchar_t dst, ushort_t cmd, uchar_t* pParams, uint_t paramsLen);
/*
 * 从CommBuffer中识别并填充一个包，返回实际填充的字节数
 * 注意1：如果识别失败，会清除CommBuffer中的异常数据，并返回0
 * 注意2: 如果识别成功，仅返回第一包有效数据
*/
uint_t FillCommPacketFromCommBuffer(uchar_t* pPackBuff, uint_t buffSz, uint_t commBuffId);

uchar_t GetPackFieldCtrl(uchar_t* pPackBuff);
uchar_t GetPackFieldLen(uchar_t* pPackBuff);
uchar_t GetPackFieldSrc(uchar_t* pPackBuff);
uchar_t GetPackFieldDst(uchar_t* pPackBuff);
ushort_t GetPackFieldCmd(uchar_t* pPackBuff);
uint_t  GetPackFieldParams(uchar_t* pPackBuff, uchar_t** pOutData, uint_t* outLen);
uint_t GetPackFieldParamsLen(uchar_t* pPackBuff);
uchar_t GetPackCheckSum(uchar_t* pPackBuff);


#endif
