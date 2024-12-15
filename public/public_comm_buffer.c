/**********************************************************
filename   : public_comm_buffer.c
function   : 通讯缓存（公共部分）
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/

#include "public_comm_buffer.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
 */
extern const uint_t G_COMM_BUFFERS_NUMBER;
extern COMM_BUFF_STATUS g_comm_buffers[];
//=================================================================

// 缓存对象初始化
uint_t CommBufferInit(uint_t id) {
    g_comm_buffers[id].ulHeadIdx = 0;
    g_comm_buffers[id].ulTailIdx = 0;
    g_comm_buffers[id].pauseCount = 0;
    return 0;
}

uint_t CommBuffersAllInit(void){
    uint_t id = 0;
    for (id = 0; id < G_COMM_BUFFERS_NUMBER; id++) {
        CommBufferInit(id);
    }
    return 0;
}

// 判断缓存对象是否已满
uint_t CommBufferIsFull(uint_t id) {
    if (CommBufferGetCount(id) >= COMM_BUFFER_FULL_SZ) {
        return 1;
    }
    return 0;
}

// 判断缓存对象是否为空
uint_t CommBufferIsEmpty(uint_t id) {
    if (0 == CommBufferGetCount(id)) {
        return 1;
    }
    return 0;
}

// 获取缓存对象剩余数量
uint_t CommBufferGetLeftSize(uint_t id) {
    return COMM_BUFFER_FULL_SZ - CommBufferGetCount(id);
}

// 获取缓存对象当前可用数量
uint_t CommBufferGetCount(uint_t id) {
    return (COMM_BUFFER_MAX_SZ + g_comm_buffers[id].ulTailIdx - g_comm_buffers[id].ulHeadIdx) & COMM_BUFFER_SZ_MASK;
}

// 往指定缓存追加数据
uint_t CommBufferPush(uint_t id, uchar_t* pInData, uint_t uInDataLen) {
    uint_t leftlen = CommBufferGetLeftSize(id);
    uint_t i = 0;
    if (uInDataLen > leftlen) {
        g_comm_buffers[id].pauseCount += 1;
        uInDataLen = leftlen;
    }
    for (i = 0; i < uInDataLen; i++) {
        g_comm_buffers[id].ucBuff[(g_comm_buffers[id].ulTailIdx + i) & COMM_BUFFER_SZ_MASK] = pInData[i];
    }
    g_comm_buffers[id].ulTailIdx = (g_comm_buffers[id].ulTailIdx + uInDataLen) & COMM_BUFFER_SZ_MASK;
    return uInDataLen;
}

// 从指定缓存提取数据
uint_t CommBufferPop(uint_t id, uchar_t* pOutData, uint_t uOutDataLen) {
    uint_t i = 0;
    uint_t cplen = CommBufferGetCount(id);
    if (0 == cplen) {
        return 0;
    }
    if (cplen > uOutDataLen) {
        cplen = uOutDataLen;
    }
    if (pOutData) {
        for (i = 0; i < cplen; i++) {
            pOutData[i] = g_comm_buffers[id].ucBuff[(g_comm_buffers[id].ulHeadIdx + i) & COMM_BUFFER_SZ_MASK];
        }
    }
    g_comm_buffers[id].ulHeadIdx = (g_comm_buffers[id].ulHeadIdx + cplen) & COMM_BUFFER_SZ_MASK;
    return cplen;
}

// 获取头部指针，返回内容长度
uint_t CommBufferGetHeadPoint(uint_t id, uchar_t** pOutData) {
    *pOutData = g_comm_buffers[id].ucBuff + g_comm_buffers[id].ulHeadIdx;
    return CommBufferGetCount(id);
}

// 获取指定位置的字节内容
uchar_t CommBufferGetChar(uint_t id, uint_t posIdx) {
    return *(g_comm_buffers[id].ucBuff + ((g_comm_buffers[id].ulHeadIdx + posIdx) & COMM_BUFFER_SZ_MASK));
}
