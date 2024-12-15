
/**********************************************************
filename   : byte_sequence.c
function   : 字节序转换
author     : 谢令
created    : 2022-11-16
comment    :
**********************************************************/
#include "byte_sequence.h"

/*
** 初始化时调用initByteSequence，记录本机字节序是否为小端字节序
** 将结果保存在 isLittleEndian 变量???
*  通过 IsLittleEndian 接口获取此结???
*/ 

/*
** 主机字节序识别
*/
static int isLittleEndian = 1;
void InitByteSequence(void) {
    union {
        short value;
        char union_bytes[sizeof(short)];
    } test;
    test.value = 0x0102;
    if ((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2)) {
        isLittleEndian = 0;
    }
    else {
        isLittleEndian = 1;
    }
}

int IsLittleEndian(void) { 
	return isLittleEndian;
}

int IsBigEndian(void){
    return !isLittleEndian;
}

unsigned char ushort_to_little_endian_L(unsigned short x){
    return (unsigned char)(x & 0xFF);
}

unsigned char ushort_to_big_endian_L(unsigned short x){
    return (unsigned char)((x & 0xFF00) >> 8);
}

unsigned char uchar_to_little_endian_H(unsigned short x){
    return (unsigned char)((x & 0xFF00) >> 8);
}

unsigned char uchar_to_big_endian_H(unsigned short x){
    return (unsigned char)(x & 0xFF);
}

void ushort_to_big_endian(unsigned short x, unsigned char* p){
    p[0] = (x >> 8) & 0xFF;
    p[1] = x&0xFF;
}

unsigned short ushort_from_little_end(unsigned char chL, unsigned char chH) {
    return ((unsigned short)chH << 8) + (chL);
}

unsigned short ushort_from_big_end(unsigned char chL, unsigned char chH) {
    return ((unsigned short)chL << 8) + (chH);
}

unsigned int uint_from_little_end(unsigned char* pCh, unsigned int* uiVal) {
    *uiVal = ((unsigned int)pCh[3] << 24) + ((unsigned int)pCh[2] << 16) + ((unsigned int)pCh[1] << 8) + pCh[0];
    return 0;
}

unsigned int uint_from_big_end(unsigned char* pCh, unsigned int* uiVal) {
    *uiVal = ((unsigned int)pCh[0] << 24) + ((unsigned int)pCh[1] << 16) + ((unsigned int)pCh[2] << 8) + pCh[3];
    return 0;
}
