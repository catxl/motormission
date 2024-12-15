/*!
    \file  gd32f303c_lcd_eval.h
    \brief LCD driver header file

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

#ifndef GD32F303C_LCD_EVAL_H
#define GD32F303C_LCD_EVAL_H

#include "gd32f30x.h"


/* char format struct definitions */
typedef struct
{   
    uint16_t font;              /*!< the type of font */
    uint16_t direction;         /*!< the direction of char */
    uint16_t char_color;        /*!< the color of char */
    uint16_t bk_color;          /*!< the color of backgroud */
}char_format_struct;

/* lcd peripheral initialize */
void exmc_lcd_init(void);
/* initialize the LCD */
void lcd_init(void);
/* write data to the selected LCD register */
void lcd_register_write(uint16_t register_id,uint16_t value);
/* read the value of LCD register */
uint16_t lcd_register_read(uint8_t register_id);
/* write command to LCD register */
void lcd_command_write (uint16_t value);
/* prepare to write to the LCD GRAM */
void lcd_gram_write_prepare(void);
/* write RGB code to the LCD GRAM register */
void lcd_gram_write(uint16_t rgb_code);
/* read data from GRAM */
uint16_t lcd_gram_read(void);
/* set the cursor of LCD */
void lcd_cursor_set(uint16_t x,uint16_t y);
/* clear the LCD screen to the specified color */
void lcd_clear(uint16_t Color);
/* set the point according to the specified position and color */
void lcd_point_set(uint16_t x,uint16_t y,uint16_t point);
/* get the point according to the specified position */
uint16_t lcd_point_get(uint16_t x,uint16_t y);
/* set window area */
void lcd_windows_set(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y);
/* draw a horizontal line on LCD screen */
void lcd_hline_draw(uint16_t x,uint16_t start_y,uint16_t end_y,uint16_t color,uint16_t width);
/* draw a vertical line on LCD screen */
void lcd_vline_draw(uint16_t start_x,uint16_t end_x,uint16_t y,uint16_t color,uint16_t width);
/* draw a rectangle according to the specified position and color */
void lcd_rectangle_draw(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,uint16_t point);
/* fill the specified color to a rectangle */
void lcd_rectangle_fill(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,uint16_t color);
/* draw a picture on LCD screen according to the specified position */
void lcd_picture_draw(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,uint16_t *pic);
/* display a char on LCD screen according to the specified position */
void lcd_char_display(uint16_t x,uint16_t y,uint8_t c,char_format_struct c_format);

#endif /* GD32F303C_LCD_EVAL_H */
