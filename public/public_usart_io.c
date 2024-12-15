/**********************************************************
filename   : public_usart_io.c
function   : ����ͨѶ�ӿڣ��������֣�
author     : л��
created    : 2022-11-20
comment    :
**********************************************************/
#include "public_usart_io.h"
#include "gd32f30x.h"
#include "comm_buffer.h"

//=================================================================
/*
 * ��Ҫ�ھ��嵥Ԫ��ʵ�ֵı�������
*/
extern const uint_t G_USART_PORT_NUMBER;      // ���ô�������
extern USART_CONFIG g_cfg_usart[];            // ���ô���
void gpio_config_init(void);                  // GPIO��ʼ��
//=================================================================


void usart_io_init(void){
    uint_t i = 0;
    gpio_config_init();
    for(i=0; i<G_USART_PORT_NUMBER; i++){
        // ���ô��ڵĹ�������
        usart_deinit(g_cfg_usart[i].usart);
        usart_baudrate_set(g_cfg_usart[i].usart, g_cfg_usart[i].baudrate);       // ������  9600
        usart_word_length_set(g_cfg_usart[i].usart, USART_WL_8BIT);              // ֡�����ֳ� USART_WL_9BIT   //8bit
        usart_stop_bit_set(g_cfg_usart[i].usart, USART_STB_1BIT);                // ֹͣλ
        usart_parity_config(g_cfg_usart[i].usart, USART_PM_NONE);                // ��żУ��λ USART_PM_EVEN
        usart_hardware_flow_rts_config(g_cfg_usart[i].usart, USART_RTS_DISABLE); // Ӳ��������RTS
        usart_hardware_flow_cts_config(g_cfg_usart[i].usart, USART_CTS_DISABLE); // Ӳ��������CTS
        usart_receive_config(g_cfg_usart[i].usart, USART_RECEIVE_ENABLE);        // ʹ�ܽ���
        usart_transmit_config(g_cfg_usart[i].usart, USART_TRANSMIT_ENABLE);      // ʹ�ܷ���
        usart_enable(g_cfg_usart[i].usart);                                      // ʹ�ܴ���

        CommBufferInit(g_cfg_usart[i].recvCommBuffId);
        CommBufferInit(g_cfg_usart[i].sendCommBuffId);
    }
    return;
}

uint_t usart_io_send(uint_t id, uchar_t *pData, uint_t sz){
    uint_t pushLen = CommBufferPush(g_cfg_usart[id].sendCommBuffId, pData, sz);
    uchar_t ucUartChar = 0;
    // ���ж�
    usart_interrupt_enable(g_cfg_usart[id].usart, USART_INT_TC);
    // ��ʼ��������: ����һ���ֽں������ֽ����жϳ����������
    if(CommBufferPop(g_cfg_usart[id].sendCommBuffId, &ucUartChar, 1) > 0){
        usart_data_transmit(g_cfg_usart[id].usart, ucUartChar);
    }
    return 0;
}

uint_t usart_io_read(uint_t id, uchar_t *pData, uint_t sz){
    return CommBufferPop(g_cfg_usart[id].usart, pData, sz);
}
