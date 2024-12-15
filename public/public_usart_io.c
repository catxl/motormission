/**********************************************************
filename   : public_usart_io.c
function   : 串口通讯接口（公共部分）
author     : 谢令
created    : 2022-11-20
comment    :
**********************************************************/
#include "public_usart_io.h"
#include "gd32f30x.h"
#include "comm_buffer.h"

//=================================================================
/*
 * 需要在具体单元里实现的变量定义
*/
extern const uint_t G_USART_PORT_NUMBER;      // 配置串口数量
extern USART_CONFIG g_cfg_usart[];            // 配置串口
void gpio_config_init(void);                  // GPIO初始化
//=================================================================


void usart_io_init(void){
    uint_t i = 0;
    gpio_config_init();
    for(i=0; i<G_USART_PORT_NUMBER; i++){
        // 配置串口的工作参数
        usart_deinit(g_cfg_usart[i].usart);
        usart_baudrate_set(g_cfg_usart[i].usart, g_cfg_usart[i].baudrate);       // 波特率  9600
        usart_word_length_set(g_cfg_usart[i].usart, USART_WL_8BIT);              // 帧数据字长 USART_WL_9BIT   //8bit
        usart_stop_bit_set(g_cfg_usart[i].usart, USART_STB_1BIT);                // 停止位
        usart_parity_config(g_cfg_usart[i].usart, USART_PM_NONE);                // 奇偶校验位 USART_PM_EVEN
        usart_hardware_flow_rts_config(g_cfg_usart[i].usart, USART_RTS_DISABLE); // 硬件流控制RTS
        usart_hardware_flow_cts_config(g_cfg_usart[i].usart, USART_CTS_DISABLE); // 硬件流控制CTS
        usart_receive_config(g_cfg_usart[i].usart, USART_RECEIVE_ENABLE);        // 使能接收
        usart_transmit_config(g_cfg_usart[i].usart, USART_TRANSMIT_ENABLE);      // 使能发送
        usart_enable(g_cfg_usart[i].usart);                                      // 使能串口

        CommBufferInit(g_cfg_usart[i].recvCommBuffId);
        CommBufferInit(g_cfg_usart[i].sendCommBuffId);
    }
    return;
}

uint_t usart_io_send(uint_t id, uchar_t *pData, uint_t sz){
    uint_t pushLen = CommBufferPush(g_cfg_usart[id].sendCommBuffId, pData, sz);
    uchar_t ucUartChar = 0;
    // 开中断
    usart_interrupt_enable(g_cfg_usart[id].usart, USART_INT_TC);
    // 开始发送数据: 发送一个字节后，其他字节由中断程序继续发送
    if(CommBufferPop(g_cfg_usart[id].sendCommBuffId, &ucUartChar, 1) > 0){
        usart_data_transmit(g_cfg_usart[id].usart, ucUartChar);
    }
    return 0;
}

uint_t usart_io_read(uint_t id, uchar_t *pData, uint_t sz){
    return CommBufferPop(g_cfg_usart[id].usart, pData, sz);
}
