/**********************************************************
filename   : common.h
function   : 
author     : 谢令
created    : 2022-11-18
comment    :
**********************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

typedef unsigned int uint_t;
typedef unsigned char uchar_t;
typedef signed char char_t;
typedef unsigned short int ushort_t;
typedef unsigned long ulong_t;
typedef unsigned long long ulonglong_t;
typedef ulong_t time_ul_t;

typedef unsigned int uint32;
typedef unsigned char uchar;
typedef unsigned short int uint16;
typedef unsigned long ulong32;

// 系统单元编号
enum SYS_UINT_CODE {
    SYS_U_PC = 0,                        // PC
    SYS_U_MAINCTRL = 1,                  // 主控
    SYS_U_DEMO = 2,                      // 示例单元
};

#define THIS_UNIT_VERSION 0x2025
#define MYID SYS_U_DEMO  // 自己个儿的单元ID号

#define ROTATE_POS_NUMBER 72

#define FPGA(addr) (*((volatile unsigned short int*)(0x60000000 + (addr << 1))))

#define MAX_FRAME_LEN 128  // 最长帧长度定义

#endif
