/*!
    \file    gd32f303c_eval.c
    \brief   firmware functions to manage leds, keys, COM ports

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

#include <gd32f30x.h>
#include "leds.h"

/* eval board low layer led */
#define LEDn 4U

#define LED2_PIN GPIO_PIN_1
#define LED2_GPIO_PORT GPIOA
#define LED2_GPIO_CLK RCU_GPIOA

#define LED3_PIN GPIO_PIN_2
#define LED3_GPIO_PORT GPIOC
#define LED3_GPIO_CLK RCU_GPIOC

#define LED4_PIN GPIO_PIN_0
#define LED4_GPIO_PORT GPIOE
#define LED4_GPIO_CLK RCU_GPIOE

#define LED5_PIN GPIO_PIN_1
#define LED5_GPIO_PORT GPIOE
#define LED5_GPIO_CLK RCU_GPIOE

#define COMn 3U

#define EVAL_COM1_CLK RCU_USART0
#define EVAL_COM1_TX_PIN GPIO_PIN_9
#define EVAL_COM1_RX_PIN GPIO_PIN_10
#define EVAL_COM1_GPIO_PORT GPIOA
#define EVAL_COM1_GPIO_CLK RCU_GPIOA

#define EVAL_COM2_CLK RCU_USART1
#define EVAL_COM2_TX_PIN GPIO_PIN_2
#define EVAL_COM2_RX_PIN GPIO_PIN_3
#define EVAL_COM2_GPIO_PORT GPIOA
#define EVAL_COM2_GPIO_CLK RCU_GPIOA

#define EVAL_COM3_CLK RCU_UART3
#define EVAL_COM3_TX_PIN GPIO_PIN_10
#define EVAL_COM3_RX_PIN GPIO_PIN_11
#define EVAL_COM3_GPIO_PORT GPIOC
#define EVAL_COM3_GPIO_CLK RCU_GPIOC

#define KEYn 3U

/* wakeup push-button */
#define WAKEUP_KEY_PIN GPIO_PIN_0
#define WAKEUP_KEY_GPIO_PORT GPIOA
#define WAKEUP_KEY_GPIO_CLK RCU_GPIOA
#define WAKEUP_KEY_EXTI_LINE EXTI_0
#define WAKEUP_KEY_EXTI_PORT_SOURCE GPIO_PORT_SOURCE_GPIOA
#define WAKEUP_KEY_EXTI_PIN_SOURCE GPIO_PIN_SOURCE_0
#define WAKEUP_KEY_EXTI_IRQn EXTI0_IRQn

/* tamper push-button */
#define TAMPER_KEY_PIN GPIO_PIN_13
#define TAMPER_KEY_GPIO_PORT GPIOC
#define TAMPER_KEY_GPIO_CLK RCU_GPIOC
#define TAMPER_KEY_EXTI_LINE EXTI_13
#define TAMPER_KEY_EXTI_PORT_SOURCE GPIO_PORT_SOURCE_GPIOC
#define TAMPER_KEY_EXTI_PIN_SOURCE GPIO_PIN_SOURCE_13
#define TAMPER_KEY_EXTI_IRQn EXTI10_15_IRQn

/* user push-button */
#define USER_KEY_PIN GPIO_PIN_14
#define USER_KEY_GPIO_PORT GPIOB
#define USER_KEY_GPIO_CLK RCU_GPIOB
#define USER_KEY_EXTI_LINE EXTI_14
#define USER_KEY_EXTI_PORT_SOURCE GPIO_PORT_SOURCE_GPIOB
#define USER_KEY_EXTI_PIN_SOURCE GPIO_PIN_SOURCE_14
#define USER_KEY_EXTI_IRQn EXTI10_15_IRQn

/* private variables */
static uint32_t GPIO_PORT[LEDn] = {LED2_GPIO_PORT, LED3_GPIO_PORT,
                                   LED4_GPIO_PORT, LED5_GPIO_PORT};
static uint32_t GPIO_PIN[LEDn] = {LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN};
/*
static rcu_periph_enum COM_CLK[COMn] = {EVAL_COM1_CLK, EVAL_COM2_CLK, EVAL_COM3_CLK};
static uint32_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN, EVAL_COM2_TX_PIN, EVAL_COM3_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN, EVAL_COM2_RX_PIN, EVAL_COM3_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] = {EVAL_COM1_GPIO_PORT, EVAL_COM2_GPIO_PORT, EVAL_COM3_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {EVAL_COM1_GPIO_CLK, EVAL_COM2_GPIO_CLK, EVAL_COM3_GPIO_CLK};
*/
static rcu_periph_enum GPIO_CLK[LEDn] = {LED2_GPIO_CLK, LED3_GPIO_CLK,
                                         LED4_GPIO_CLK, LED5_GPIO_CLK};

static uint32_t KEY_PORT[KEYn] = {WAKEUP_KEY_GPIO_PORT,
                                  TAMPER_KEY_GPIO_PORT,
                                  USER_KEY_GPIO_PORT};
static uint32_t KEY_PIN[KEYn] = {WAKEUP_KEY_PIN,
                                 TAMPER_KEY_PIN,
                                 USER_KEY_PIN};
static rcu_periph_enum KEY_CLK[KEYn] = {WAKEUP_KEY_GPIO_CLK,
                                        TAMPER_KEY_GPIO_CLK,
                                        USER_KEY_GPIO_CLK};
static exti_line_enum KEY_EXTI_LINE[KEYn] = {WAKEUP_KEY_EXTI_LINE,
                                             TAMPER_KEY_EXTI_LINE,
                                             USER_KEY_EXTI_LINE};
static uint8_t KEY_PORT_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PORT_SOURCE,
                                        TAMPER_KEY_EXTI_PORT_SOURCE,
                                        USER_KEY_EXTI_PORT_SOURCE};
static uint8_t KEY_PIN_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PIN_SOURCE,
                                       TAMPER_KEY_EXTI_PIN_SOURCE,
                                       USER_KEY_EXTI_PIN_SOURCE};
static uint8_t KEY_IRQn[KEYn] = {WAKEUP_KEY_EXTI_IRQn,
                                 TAMPER_KEY_EXTI_IRQn,
                                 USER_KEY_EXTI_IRQn};

/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_init(led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    //		rcu_periph_clock_enable(RCU_GPIOD);	// GOPID使能时钟
    //		GPIO_BC(GPIOD) = GPIO_PIN_13;				// 引脚写0
    //		GPIO_BOP(GPIOD) = GPIO_PIN_13;			// 引脚写1
    /* configure led GPIO port */
    gpio_init(GPIO_PORT[lednum], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_toggle(led_typedef_enum lednum)
{
    gpio_bit_write(GPIO_PORT[lednum], GPIO_PIN[lednum],
                   (bit_status)(1 - gpio_input_bit_get(GPIO_PORT[lednum], GPIO_PIN[lednum])));
}

/*!
    \brief      configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[key_num]);
    rcu_periph_clock_enable(RCU_AF);

    /* configure button pin as input */
    gpio_init(KEY_PORT[key_num], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY_PIN[key_num]);

    if (key_mode == KEY_MODE_EXTI)
    {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[key_num], 2U, 0U);

        /* connect key EXTI line to key GPIO pin */
        gpio_exti_source_select(KEY_PORT_SOURCE[key_num], KEY_PIN_SOURCE[key_num]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[key_num], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[key_num]);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t gd_eval_key_state_get(key_typedef_enum key)
{
    return gpio_input_bit_get(KEY_PORT[key], KEY_PIN[key]);
}
