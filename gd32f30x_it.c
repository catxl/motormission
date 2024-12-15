/*!
	\file    gd32f30x_it.c
	\brief   interrupt service routines

	\version 2021-03-23, V2.0.0, demo for GD32F30x
*/

/*
	Copyright (c) 2021, GigaDevice Semiconductor Inc.

	Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its contributors
	   may be used to endorse or promote products derived from this software without
	   specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f30x_it.h"
#include "can_io.h"
#include "comm_buffer.h"

static volatile time_ul_t s_systick_counter = 0;


/*!
	\brief      this function handles NMI exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void NMI_Handler(void)
{
}

/*!
	\brief      this function handles HardFault exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void HardFault_Handler(void)
{
	/* if Hard Fault exception occurs, go to infinite loop */
	while (1);
}

/*!
	\brief      this function handles MemManage exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void MemManage_Handler(void)
{
	/* if Memory Manage exception occurs, go to infinite loop */
	while (1);
}

/*!
	\brief      this function handles BusFault exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void BusFault_Handler(void)
{
	/* if Bus Fault exception occurs, go to infinite loop */
	while (1);
}

/*!
	\brief      this function handles UsageFault exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void UsageFault_Handler(void)
{
	/* if Usage Fault exception occurs, go to infinite loop */
	while (1)
		;
}

/*!
	\brief      this function handles SVC exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void SVC_Handler(void)
{
}

/*!
	\brief      this function handles DebugMon exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
	\brief      this function handles PendSV exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void PendSV_Handler(void)
{
}

/*!
	\brief      this function handles SysTick exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void SysTick_Handler(void)
{
	s_systick_counter += 1;
}

time_ul_t GetCurrentSysTickCounter(){
	return s_systick_counter;
}

void USBD_LP_CAN0_RX0_IRQHandler(void)
{
	int num = 0;

	if (RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MTF0))
	{
	}
	if (RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_RFL0))
	{
		/* check the receive message */
		//	    can_message_receive(CAN0, CAN_FIFO0, &receive_message);
		num = can_receive_message_length_get(CAN0, CAN_FIFO0);
		receive_data(num);
	}
}

/*!
	\brief      this function handles USART0 exception
	\param[in]  none
	\param[out] none
	\retval     none
*/
void UART3_IRQHandler(void)
{
	uchar_t ucUartChar; // 作为接收和发送数据的临时缓冲区
	//	uchar_t  temppage;

	if (RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE))
	{
		/* read one byte from the receive data register */
		ucUartChar = (uchar_t)usart_data_receive(UART3);
		CommBufferPush(USART3_RECV_BUFFER, &ucUartChar, 1);
	}
	if (RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_TC)) // USART_INT_FLAG_TBE
	{
		usart_flag_clear(UART3, USART_FLAG_TC); // USART_INT_FLAG_TBE
		if (CommBufferPop(USART3_SEND_BUFFER, &ucUartChar, 1) > 0)
		{
			usart_data_transmit(UART3, ucUartChar);
		}
	}
}
