/*
 * ILI9341_Primitives.c
 *
 *  Created on: 12 февр. 2018 г.
 *      Author: ellissar
 *
 *      Здесь находятся функции для рисования основных геометрических примитивов и функции работы с символами.
 *      Порядок подключения и работы с дисплеем описан в файле ILI9341.h
 *
 */


#include "ILI9341_draw.h"


//Ограничивает координаты рабочей области по оси Х
void ili9341_Set_Column(uint16_t start_column, uint16_t end_colunm)
{
	ili9341_Send_Cmd(LCD_COLUMN_ADDR);		//Column Address Set
	ili9341_Send_Data(start_column);
	ili9341_Send_Data(end_colunm);
}


//Ограничивает координаты рабочей области по оси Y
void ili9341_Set_Page(uint16_t start_page, uint16_t end_page)
{
	ili9341_Send_Cmd(LCD_PAGE_ADDR);		//Page Address Set
	ili9341_Send_Data(start_page);
	ili9341_Send_Data(end_page);
}


//Ограничивает координаты рабочей области
void ili9341_Set_XY(uint16_t x, uint16_t y)
{
	ili9341_Set_Column(x, x);
	ili9341_Set_Page(y, y);
}


//Отрисовывает пиксель по заданным координатам
void ili9341_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
	ili9341_Set_XY(x, y);
	ili9341_Send_Cmd(LCD_GRAM);				//Memory Write
	ili9341_Write_Data(color);
}


//Если А между В и С возвращает А.
//Если А выходит за границы B и C, то возвращает границу (В или С) к которой А ближе.
uint16_t constrain(uint16_t a, uint16_t b, uint16_t c)
{
	if (a < b)
	{
		return b;
	}
	if (c < a)
	{
		return c;
	}
	else return a;
}


//Закрасить прямоугольную область.
//Задаются координаты левого верхнего и правого нижнего углов.
void ili9341_Fill_Rect(uint16_t x_left, uint16_t x_right, uint16_t y_up, uint16_t y_down, uint16_t color)
{
	unsigned long xy=0;
	unsigned long i=0;
	if(x_left > x_right)
	{
		x_left = x_left^x_right;       //если координата левого края больше
		x_right = x_left^x_right;      //координаты правого края они поменяются
		x_left = x_left^x_right;       //местами, было x_left = 5 x_right = 3
					       	   	   	   //стало x_left = 3 x_right = 5
	}
	if(y_up > y_down)
	{
		y_up = y_up^y_down;				//то же самое для оси y
		y_down = y_up^y_down;			//название этой операции
		y_up = y_up^y_down;				//"swap без временной переменной"
	}
	//контролируем, что бы передаваемые в функцию координаты
	//входили в область допустимых значений
	x_left = constrain(x_left, ili9341_MIN_X, ili9341_MAX_X);
	x_right = constrain(x_right, ili9341_MIN_X, ili9341_MAX_X);
	y_up = constrain(y_up, ili9341_MIN_Y, ili9341_MAX_Y);
	y_down = constrain(y_down, ili9341_MIN_Y, ili9341_MAX_Y);

	xy = (x_right - x_left+1);			//рассчитываем количество точек
	xy = xy*(y_down - y_up+1);			//которое надо закрасить

	ili9341_Set_Column(x_left, x_right);	    //задаём рабочую область по x
	ili9341_Set_Page(y_up, y_down);				//задаём рабочую область по y
	ili9341_Send_Cmd(0x2C);						//будем писать в видео ОЗУ

	for(i=0; i < xy; i++)
	{
		ili9341_Write_Data(color);				//передаём кодировку цвета
	}
}


//Закрашенный прямоугольник или квадрат нужного размера.
//Задаются координаты левого верхнего угла, длина и ширина в пикселях.
void ili9341_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color)
{
	if(length && width)
	{
		ili9341_Fill_Rect(x, x+length - 1, y, y+width - 1, color);
	}
}


//Вывод одного символа
void ili9341_Draw_Char(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, uint8_t ascii, uint8_t size)
{
	for (int i = 0; i < FONT_Y; i++ )
	{
		for(uint8_t f = 0; f < FONT_X; f++)
		{
			if((simbols_f[ascii-0x20][i]>>(7-f))&0x01)
			{
				ili9341_Fill_Rectangle(x+f*size, y+i*size, size, size, color);
			}
			else
			{
				ili9341_Fill_Rectangle(x+f*size, y+i*size, size, size, phone);
			}
		}
	}
}


//Вывод строки
void ili9341_Draw_String(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, char *string, uint8_t size)
{
    //определить конец строки очень просто если знать, что она ВСЕГДА заканчивается нулём
	while(*string)
	{
        /* проверяем не вылезем ли мы за пределы экрана при отрисовке следующего символа,
         * если да, то переходим на следующую строчку */
		if((x + FONT_X) > ili9341_MAX_X)
		{
			x = 1;
			y = y + FONT_X*size;
		}
		ili9341_Draw_Char(x, y, color, phone,*string, size);	//выводим символ
		x += FONT_X*size;		//изменяем координату для отрисовки следующего символа
		*string++;				//увеличиваем значение указателя, чтобы он ссылался на следующий символ
	}
}


//Линия с произволным наклоном по алгоритму Брезенхема
void ili9341_Draw_Line( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{

	int x = x1-x0;
	int y = y1-y0;
	int dx = abs(x), sx = x0<x1 ? 1 : -1;
	int dy = -abs(y), sy = y0<y1 ? 1 : -1;
	int err = dx+dy, e2;
	for (;;){
		ili9341_Draw_Pixel(x0,y0,color);
		e2 = 2*err;
		if (e2 >= dy) {
			if (x0 == x1) break;
			err += dy; x0 += sx;
		}
		if (e2 <= dx) {
			if (y0 == y1) break;
			err += dx; y0 += sy;
		}
	}
}


//Горизонтальная линия
void ili9341_Draw_Horizontal_Line(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
	ili9341_Set_Column(x,x + length);
	ili9341_Set_Page(y,y);
	ili9341_Send_Cmd(0x2c);
	for(int i=0; i<length; i++)
		ili9341_Write_Data(color);
}


//Вертикальная линия
void ili9341_Draw_Vertical_Line(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
	ili9341_Set_Column(x,x);
	ili9341_Set_Page(y,y+length);
	ili9341_Send_Cmd(0x2c);
	for(int i=0; i<length; i++)
		ili9341_Write_Data(color);
}


//Незакрашенный прямоугольник
void ili9341_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color)
{
	ili9341_Draw_Horizontal_Line(x, y, length, color);
	ili9341_Draw_Horizontal_Line(x, y + width-1, length, color);
	ili9341_Draw_Vertical_Line(x, y, width,color);
	ili9341_Draw_Vertical_Line(x + length -1, y, width,color);
}


//Незакрашенный треугольник
void ili9341_Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	ili9341_Draw_Line( x1, y1, x2, y2, color);
	ili9341_Draw_Line( x2, y2, x3, y3, color);
	ili9341_Draw_Line( x3, y3, x1, y1, color);
}


//Окружность незакрашенная
void ili9341_Draw_Circle(uint16_t pos_x, uint16_t pos_y, uint8_t r, uint16_t color)
{
	int x = -r, y = 0, err = 2-2*r, e2;
	do {
		ili9341_Draw_Pixel(pos_x-x, pos_y+y, color);
		ili9341_Draw_Pixel(pos_x+x, pos_y+y, color);
		ili9341_Draw_Pixel(pos_x+x, pos_y-y, color);
		ili9341_Draw_Pixel(pos_x-x, pos_y-y, color);
		e2 = err;
		if (e2 <= y) {
			err += ++y*2+1;
			if (-x == y && e2 <= x) e2 = 0;
		}
		if (e2 > x) err += ++x*2+1;
	} while (x <= 0);
}


//Окружность закрашенная
void ili9341_Fill_Circle(uint16_t pos_x, uint16_t pos_y, uint8_t r, uint16_t color)
{
	int x = -r, y = 0, err = 2-2*r, e2;
	do {

		ili9341_Draw_Vertical_Line(pos_x-x, pos_y-y, 2*y, color);
		ili9341_Draw_Vertical_Line(pos_x+x, pos_y-y, 2*y, color);

		e2 = err;
		if (e2 <= y) {
			err += ++y*2+1;
			if (-x == y && e2 <= x) e2 = 0;
		}
		if (e2 > x) err += ++x*2+1;
	} while (x <= 0);

}
