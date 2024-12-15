/**********************************************************
filename   : main.c
function   : 主程序
author     : 谢令
created    : 2022-11-22
comment    :
**********************************************************/

#include "gd32f30x.h"
#include <stdio.h>
#include "common.h"

// io
#include "can_io.h"
#include "usart_io.h"
#include "comm_proc.h"
#include "eeprom.h"

// workers
#include "leds.h"
#include "lcds.h"
#include "step_motors.h"
#include "dc_motors.h"
#include "timers.h"
#include "photosensors.h"

// processors
#include "sensor_counter_processors.h"

// mission
#include "all_missions.h"

void fpga_init() {
    delay_ms(1000);
    gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_bit_set(GPIOD, GPIO_PIN_13);
}

int main(void)
{
    // init IO
    can_io_init();
    usart_io_init();

    // initialize LED
    gd_eval_led_init(LED2);

    /* configure systick */
    systick_config();
    /* configure the EXMC access mode */
    exmc_lcd_init();
    /* configure the SPI for EEPROM*/
    spi_flash_init();

    // 启动计时器
    TimerStart(TIMER_LED_INTERVAL);
    TimerStart(TIMER_CLEAN_DELAY_INTERVAL);

    // init fpga
    fpga_init();

    while (1){
        // 指示灯输出
        if(TimerTap(TIMER_LED_INTERVAL)>0){
            gd_eval_led_toggle(LED2);
        }

        CommPackProcess();                 // 通信协议解释
        ReadAllPhotoSensorStatus();        // 刷新传感器状态
        SensorCounterProcessorsAllTick();  // 传感器计数器心跳
        AllMissionsTick();                 // 任务心跳
    }
}


/* retarget the C library printf function to the usart */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(UART3, (uint8_t)ch);
    while (RESET == usart_flag_get(UART3, USART_FLAG_TBE));
    return ch;
}
