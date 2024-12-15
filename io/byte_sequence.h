/**********************************************************
filename   : byte_sequence.h
function   : 字节序转换
author     : 谢令
created    : 2022-11-16
comment    :
**********************************************************/

#ifndef __BYTE_SEQUENCE_H__
#define __BYTE_SEQUENCE_H__

/*
** 初始化时调用 InitByteSequence，记录本机字节序是否为小端字节序
*/ 
void InitByteSequence(void);

/*
** 判断本机是否是小端字节序
*/
int IsLittleEndian(void);
int IsBigEndian(void);

/*
** short: 主机字节序（输入参数）--> 小端字节序（返回值）
*/
unsigned char ushort_to_little_endian_L(unsigned short x);
unsigned char uchar_to_little_endian_H(unsigned short x);

/*
** short: 主机字节序（输入参数）--> 大端字节序（返回值）
*/
unsigned char ushort_to_big_endian_L(unsigned short x);
unsigned char uchar_to_big_endian_H(unsigned short x);
void ushort_to_big_endian(unsigned short x, unsigned char* p);

/*
** short: 大/小端字节序 --> 主机字节序
*/
unsigned short ushort_from_little_end(unsigned char chL, unsigned char chH);
unsigned short ushort_from_big_end(unsigned char chL, unsigned char chH);

/*
** 大/小端字节序 --> 主机字节序
*/
unsigned int uint_from_little_end(unsigned char* pCh, unsigned int* uiVal);
unsigned int uint_from_big_end(unsigned char* pCh, unsigned int* ullVal);

#endif
