/**********************************************************
filename   : usart_io.c
function   : ����ͨѶ�ӿ�
author     : л��
created    : 2022-11-20
comment    :
**********************************************************/
#include "usart_io.h"
#include "gd32f30x.h"
#include "comm_buffer.h"

//=================================================================
/*
 * ����Ԫʹ�õ��Ĵ��ڵ�ʵ�ֶ���
*/
const uint_t G_USART_PORT_NUMBER = USART_PORT_NUMBER;                    // ���ô�������
USART_CONFIG g_cfg_usart[G_USART_PORT_NUMBER] = {                        // ���ô��ڻ�����Ϣ
    {UART3,    115200,   USART3_RECV_BUFFER,    USART3_SEND_BUFFER}
};

void gpio_config_init(void){
    // UART3
    rcu_periph_clock_enable(RCU_GPIOC); // enable GPIO clock
    rcu_periph_clock_enable(RCU_UART3); // enable USART clock
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10); // connect port to USARTx_Tx
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11); // connect port to USARTx_Rx
    usart_interrupt_enable(UART3, USART_INT_RBNE); // enable USART3 receive interrupt
    usart_interrupt_enable(UART3, USART_INT_TC); // enable USART3 transmit interrupt
    nvic_irq_enable(UART3_IRQn, 3, 1);
}
//=================================================================
