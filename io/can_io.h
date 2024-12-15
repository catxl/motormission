/**********************************************************
filename   : can_io.h
function   : CAN总线通讯接口
author     : 谢令
created    : 2022-11-20
comment    :
**********************************************************/
#ifndef _CAN_IO_H_								
#define _CAN_IO_H_
#include "common.h"

// CAN 初始化、发送、接收 接口
void can_io_init(void);
uint_t can_io_send(uchar_t* pData, uint_t size);

/*
 * can中断处理函数中调用: receive_data
*/
void receive_data (ushort_t MsgNum);

#endif


