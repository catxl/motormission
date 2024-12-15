/**********************************************************
filename   : comm_buffer.c
function   : 通讯缓存
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/

#include "comm_buffer.h"

//=================================================================
/*
 * 本单元使用到的缓冲区的实现定义
*/
const uint_t G_COMM_BUFFERS_NUMBER = COMM_BUFFERS_NUMBER;  // 配置缓存的个数
COMM_BUFF_STATUS g_comm_buffers[G_COMM_BUFFERS_NUMBER];
//=================================================================

