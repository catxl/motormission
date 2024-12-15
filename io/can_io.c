
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "can_io.h"
#include "gd32f30x.h"
#include <gd32f30x_can.h>
#include <string.h>

#include "comm_buffer.h"
//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------


can_receive_message_struct receive_message;
can_trasnmit_message_struct transmit_message;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gpio_config(void)
{
    /* enable can clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOA);

    /* configure CAN0 GPIO, CAN0_TX(PD1) and CAN0_RX(PD0) */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    //    gpio_pin_remap_config(GPIO_CAN_FULL_REMAP,ENABLE);
}

/*!
\brief      configure the nested vectored interrupt controller
\param[in]  none
\param[out] none
\retval     none
*/
void nvic_config(void)
{
    /* configure CAN0 NVIC */
    nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn,3,1);
}

void can_networking_init(void)
{
    can_parameter_struct can_parameter;
    can_filter_parameter_struct can_filter;
    /* initialize CAN structures */
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
    /* initialize CAN register */
    can_deinit(CAN0);

    /* initialize CAN */
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = DISABLE;
    can_parameter.auto_wake_up = DISABLE;
    can_parameter.no_auto_retrans = DISABLE;
    can_parameter.rec_fifo_overwrite = DISABLE;
    can_parameter.trans_fifo_order = DISABLE;
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_6TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_5TQ;
    /* baudrate 500kbps */
    can_parameter.prescaler = 6;
    can_init(CAN0, &can_parameter);

    /* initialize filter */
    /* CAN0 filter number */
    can_filter.filter_number = 0;
    can_filter.filter_mode = CAN_FILTERMODE_LIST;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = MYID<<3;
    can_filter.filter_list_low =  SYS_U_MAINCTRL<<3 | CAN_FF_EXTENDED | CAN_FT_DATA;
    //    can_filter.filter_mask_high = 0x0000;
    //    can_filter.filter_mask_low = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);
}


//-----------------------------------------------------------------------------
// Function_Init
//-----------------------------------------------------------------------------
void CAN_Init(void)
{
    /* configure GPIO */
    gpio_config();
    /* configure NVIC */
    nvic_config();
    /* initialize CAN and CAN filter */
    can_networking_init();
    /* enable CAN receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INTEN_RFNEIE0);
}


void Transmit_Frame (uchar_t *DATA_Point, int size, ushort_t IDReceiNum,ushort_t IDTransNum)
{
    uint8_t mailbox_number;
    FlagStatus status;
    can_flag_enum flag;
    /* initialize transmit message */
    transmit_message.tx_sfid = 0x321;
    transmit_message.tx_efid = ((uint32_t)IDReceiNum<<16)|IDTransNum;
    transmit_message.tx_ft = CAN_FT_DATA;
    transmit_message.tx_ff = CAN_FF_EXTENDED;
    transmit_message.tx_dlen = size;
    //    printf("\r\nplease press the Tamper key to transmit data!\r\n");
    memcpy(transmit_message.tx_data, DATA_Point, size);
    do
    {
        mailbox_number = can_message_transmit(CAN0, &transmit_message);
    }while(CAN_NOMAILBOX == mailbox_number);

    if(CAN_MAILBOX0 == mailbox_number)
    flag = CAN_FLAG_MTF0;
    else if(CAN_MAILBOX1 == mailbox_number)
    flag = CAN_FLAG_MTF1;
    else if(CAN_MAILBOX2 == mailbox_number)
    flag = CAN_FLAG_MTF2;
    else
    flag = CAN_FLAG_MTF0;

    do
    {
        status = can_flag_get(CAN0, flag);
    }while(SET != status);
    can_flag_clear(CAN0, flag);
}

void receive_data (ushort_t MsgNum)
{
    while(MsgNum--){
        can_message_receive(CAN0, CAN_FIFO0, &receive_message);
        if((CAN_FF_EXTENDED == receive_message.rx_ff) ){
            CommBufferPush(CAN_RECV_BUFFER, receive_message.rx_data, receive_message.rx_dlen);
        }
    }

}


void can_io_init(void){
    CAN_Init();
}

uint_t can_io_send(uchar_t* pData, uint_t size){
    uint_t i = 0;
    for(i = 0; i < size/8; i++){
        Transmit_Frame(&pData[8 * i], 8, SYS_U_MAINCTRL, MYID);
    }
    Transmit_Frame(&pData[8 * i], size%8, SYS_U_MAINCTRL, MYID);
    return size;
}
