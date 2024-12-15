/**********************************************************
filename   : public_usart_io.h
function   : ����ͨѶ�ӿڣ��������֣�
author     : л��
created    : 2022-11-20
comment    :
**********************************************************/
#ifndef _PUBLIC_USART_IO_H_								
#define _PUBLIC_USART_IO_H_
#include "common.h"

typedef struct{
    uint_t usart;              // USART ��ţ�USARTx(0~2)/UARTx(3~4)
    uint_t baudrate;
    uint_t recvCommBuffId;
    uint_t sendCommBuffId;
} USART_CONFIG;

void usart_io_init(void);
uint_t usart_io_send(uint_t id, uchar_t *pData, uint_t sz);
uint_t usart_io_read(uint_t id, uchar_t *pData, uint_t sz);


#endif


