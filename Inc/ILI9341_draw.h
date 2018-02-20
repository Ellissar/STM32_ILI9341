/*
 * ILI9341_Primitives.h
 *
 *  Created on: 12 февр. 2018 г.
 *      Author: ellissar
 */

#ifndef ILI9341_DRAW_H_
#define ILI9341_DRAW_H_


#include "ILI9341_colors.h"
#include "ILI9341_core.h"
#include "ILI9341_font.h"


//--------------------------------------------------------------------
//Вспомогательные функции
//--------------------------------------------------------------------
void ili9341_Set_Column(uint16_t start_column, uint16_t end_colunm);
void ili9341_Set_Page(uint16_t start_page, uint16_t end_page);
void ili9341_Set_XY(uint16_t x, uint16_t y);
void ili9341_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);
uint16_t constrain(uint16_t a, uint16_t b, uint16_t c);


//--------------------------------------------------------------------
//Основные функции
//--------------------------------------------------------------------
void ili9341_Fill_Screen(uint16_t x_left, uint16_t x_right, uint16_t y_up, uint16_t y_down, uint16_t color);
void ili9341_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color);
void ili9341_Draw_Char(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, uint8_t ascii, uint8_t size);
void ili9341_Draw_String(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, char *string, uint8_t size);
void ili9341_Draw_Line( uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1,uint16_t color);
void ili9341_Draw_Horizontal_Line(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void ili9341_Draw_Vertical_Line(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void ili9341_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color);
void ili9341_Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void ili9341_Draw_Circle(uint16_t pos_x, uint16_t pos_y, uint8_t r, uint16_t color);
void ili9341_Fill_Circle(uint16_t pos_x, uint16_t pos_y, uint8_t r, uint16_t color);


#endif /* ILI9341_DRAW_H_ */
