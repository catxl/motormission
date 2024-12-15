/**********************************************************
filename   : public_comm_buffer.h
function   : 通讯缓存（公共部分）
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#ifndef _PUBLIC_COMM_BUFFER_H_
#define _PUBLIC_COMM_BUFFER_H_

#include "common.h"

// 定义每个缓存的最大长度(1<<8 = 256)
#define COMM_BUFFER_MAX_SZ (1<<8)
#define COMM_BUFFER_SZ_MASK (COMM_BUFFER_MAX_SZ-1)
#define COMM_BUFFER_FULL_SZ COMM_BUFFER_SZ_MASK

// 缓存对象定义
typedef struct {
    unsigned char ucBuff[COMM_BUFFER_MAX_SZ];
    uint_t ulHeadIdx; 
    uint_t ulTailIdx;
    uint_t pauseCount;
} COMM_BUFF_STATUS;

// 缓存对象初始化
uint_t CommBufferInit(uint_t id);
uint_t CommBuffersAllInit(void);
// 判断缓存对象是否已满
uint_t CommBufferIsFull(uint_t id);
// 判断缓存对象是否为空
uint_t CommBufferIsEmpty(uint_t id);
// 获取缓存对象剩余数量
uint_t CommBufferGetLeftSize(uint_t id);
// 获取缓存对象当前可用数量
uint_t CommBufferGetCount(uint_t id);
// 往指定缓存追加数据
uint_t CommBufferPush(uint_t id, uchar_t* pInData, uint_t uInDataLen);
// 从指定缓存提取数据
uint_t CommBufferPop(uint_t id, uchar_t* pOutData, uint_t uOutDataLen);
// 获取头部指针，返回内容长度
uint_t CommBufferGetHeadPoint(uint_t id, uchar_t** pOutData);
// 获取指定位置的字节内容
uchar_t CommBufferGetChar(uint_t id, uint_t posIdx);

#endif
