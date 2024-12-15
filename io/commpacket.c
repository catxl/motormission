/**********************************************************
filename   : commpacket.h
function   : 通信包
author     : 谢令
created    : 2022-11-20
comment    :
**********************************************************/

#include "commpacket.h"
#include "comm_buffer.h"

#define PACKET_HEADER 0x68
#define PACKET_TAILER 0x16

/*
 * 从连续内存中识别、填充报文的接口
 * CalcCheckSum
 * IsValidPacket
 * FillCommPacket
*/
uchar_t CalcCheckSum(uchar_t* pBuff, uint_t sz){
    uchar_t chksum = 0;
    uint_t i = 0;
    for(i = 0; i<sz; i++){
        chksum += pBuff[i];
    }
    return chksum;
}

// 判断pBuff指向的内存是否是完整有效的包，如果是返回其长度，否则返回0, skipsz返回无效的数据的数量
uint_t IsValidPacket(uchar_t* pBuff, uint_t sz, uint_t* skipsz){
    uint_t i = 0;
    uchar_t dataLen = 0, checkSumInBuff = 0;
    // 检查缓冲区长度
    if(sz<9){
        return 0;
    }

    // 检查包头
    for (i = 0; i < sz && PACKET_HEADER != pBuff[i]; ++i);
    if (i > 0) {
        *skipsz = i;
        return 0;
    }

    // 判断数据长度字段是否与命令的定义匹配
    dataLen = pBuff[2];
    if( dataLen > (MAX_FRAME_LEN-5) ){
        *skipsz = 2;
        return 0;
    }
    // 判断数据是否完整
    if( sz < dataLen+5 ){
        return 0;
    }

    // 检查校验和
    checkSumInBuff = pBuff[dataLen+3];
    if(checkSumInBuff != CalcCheckSum(pBuff, dataLen+3)){
        *skipsz = (uint_t)dataLen + 3;
        return 0;
    }

    if(PACKET_TAILER != pBuff[dataLen+4]){
        *skipsz = (uint_t)dataLen + 4;
        return 0;
    }
    
    // 返回有效长度
    return dataLen+5;
}

// 向 pPackBuff 填充一个包，返回实际填充的字节数
uint_t FillCommPacket(uchar_t* pPackBuff, uint_t buffSz, uchar_t ctrl, uchar_t len, uchar_t src, uchar_t dst, ushort_t cmd, uchar_t* pParams, uint_t paramsLen){
    uint_t i = 0;
    pPackBuff[0] = PACKET_HEADER;
    pPackBuff[1] = ctrl;
    pPackBuff[2] = len;
    pPackBuff[3] = src;
    pPackBuff[4] = dst;
    pPackBuff[5] = (uchar_t)(cmd>>8);
    pPackBuff[6] = (uchar_t)(cmd&0xFF);
    if(pParams && paramsLen>0){
        for(i=0; i<paramsLen; i++){
            pPackBuff[7+i] = pParams[i];
        }
    }
    pPackBuff[7+paramsLen] = CalcCheckSum(pPackBuff, 7+paramsLen);
    pPackBuff[8+paramsLen] = PACKET_TAILER;
    return 9+paramsLen;
}

/*
 * 从连续内存中识别、填充报文的接口
 * CalcCheckSumCommBuffer
 * IsValidPacketCommBuffer
 * FillCommPacketCommBuffer
*/
uchar_t CalcCheckSumFromCommBuffer(uint_t commBuffId, uint_t startIdx, uint_t sz){
    uchar_t chksum = 0;
    uint_t i = 0;
    for(i = 0; i<sz; i++){
        chksum += CommBufferGetChar(commBuffId, startIdx+i);
    }
    return chksum;
}

// 判断CommBuffer中是否有完整有效的包，如果是返回其长度，否则返回0, skipsz返回无效的数据的数量
uint_t IsValidPacketFromCommBuffer(uint_t commBuffId, uint_t *skipsz){
    uint_t i = 0;
    uint_t sz = CommBufferGetCount(commBuffId);
    uchar_t dataLen = 0;
    // uchar_t checkSumInBuff = 0;
    // 检查缓冲区长度
    if(sz<9){
        return 0;
    }

    // 检查包头
    for (i = 0; i < sz && PACKET_HEADER != CommBufferGetChar(commBuffId, i); ++i);
    if (i > 0) {
        *skipsz = i;
        return 0;
    }

    // 判断数据长度字段是否与命令的定义匹配
    dataLen = CommBufferGetChar(commBuffId, 2);
    if( dataLen > (MAX_FRAME_LEN-5) ){
        *skipsz = 2;
        return 0;
    }
    // 判断数据是否完整
    if( sz < dataLen+5 ){
        return 0;
    }

    // 检查校验和
    //checkSumInBuff = CommBufferGetChar(commBuffId, dataLen+3);
    //if(checkSumInBuff != CalcCheckSumFromCommBuffer(commBuffId, 0, dataLen+3)){
    //    *skipsz = (uint_t)dataLen + 3;
    //    return 0;
    //}

    if(PACKET_TAILER != CommBufferGetChar(commBuffId, dataLen+4)){
        *skipsz = dataLen + 4;
        return 0;
    }
    
    // 返回有效长度
    return dataLen+5;
}

/*
 * 从CommBuffer中识别并填充一个包，返回实际填充的字节数
 * 注意1：如果识别失败，会清除CommBuffer中的异常数据，并返回0
 * 注意2: 如果识别成功，仅返回第一包有效数据
*/
uint_t FillCommPacketFromCommBuffer(uchar_t* pPackBuff, uint_t buffSz, uint_t commBuffId){
    do{
        uint_t skip_sz = 0;
        uint_t valid_sz = IsValidPacketFromCommBuffer(commBuffId, &skip_sz);
        if (0 == valid_sz){
            if (skip_sz > 0){
                CommBufferPop(commBuffId, 0, skip_sz);
                continue;
            }
            else{
                break;
            }
        }
        else{
            CommBufferPop(commBuffId, pPackBuff, valid_sz);
            return valid_sz;
        }
    }while(!CommBufferIsEmpty(commBuffId));
    return 0;
}

// 获取控制字段
uchar_t GetPackFieldCtrl(uchar_t* pPackBuff){
    return pPackBuff[1];
}

// 获取长度字段
uchar_t GetPackFieldLen(uchar_t* pPackBuff){
    return pPackBuff[2];
}

// 获取发送单元
uchar_t GetPackFieldSrc(uchar_t* pPackBuff){
    return pPackBuff[3];
}

// 获取目的单元
uchar_t GetPackFieldDst(uchar_t* pPackBuff){
    return pPackBuff[4];
}

// 获取命令
ushort_t GetPackFieldCmd(uchar_t* pPackBuff){
    return (ushort_t)(pPackBuff[5]<<8) + (ushort_t)pPackBuff[6];
}

// 获取数据段指针和长度
uint_t  GetPackFieldParams(uchar_t* pPackBuff, uchar_t** pOutData, uint_t* outLen){
    *pOutData = pPackBuff + 7;
    *outLen = GetPackFieldLen(pPackBuff) - 4;
    return *outLen;
}

// 获取参数段长度
uint_t GetPackFieldParamsLen(uchar_t* pPackBuff){
    return GetPackFieldLen(pPackBuff) - 4;
}

// 获取校验和
uchar_t GetPackCheckSum(uchar_t* pPackBuff){
    return pPackBuff[ GetPackFieldLen(pPackBuff) + 3 ];
}

