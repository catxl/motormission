/**********************************************************
filename   : comm_buffer.h
function   : 通讯缓存
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#ifndef _IO_COMM_BUFFER_H_
#define _IO_COMM_BUFFER_H_

#include "public_comm_buffer.h"

/*
* 通讯缓存标识（对象）定义
*/
typedef enum
{
    USART3_RECV_BUFFER,            // 串口3通讯接收缓存
    USART3_SEND_BUFFER,            // 串口3通讯发送缓存
    CAN_RECV_BUFFER,               // CAN口通讯接收缓存
    COMM_BUFFERS_NUMBER
}COMM_BUFFER_ID;

#endif
