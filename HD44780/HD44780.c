/*
 * HD44780.c
 *
 *  Created on: 2 февр. 2018 г.
 *      Author: ellissar
 */

#include "HD44780.h"


//Включение или выключение ножек контроллера в соответствии с младшими битами числа
void LCD_Send(uint8_t buf)
{
	if ( (buf & 0x01) == 1) {DB4_1;} else {DB4_0;}
	if ( ((buf >> 1) & 0x01) == 1) {DB5_1;} else {DB5_0;}
	if ( ((buf >> 2) & 0x01) == 1) {DB6_1;} else {DB6_0;}
	if ( ((buf >> 3) & 0x01) == 1) {DB7_1;} else {DB7_0;}
}


//Отправить инструкцию дисплею
void LCD_Send_Command (uint8_t command)
{
	RS_0;
	LCD_Send(command>>4);
	E_1;
	HAL_Delay(1);
	E_0;
	LCD_Send(command);
	E_1;
	HAL_Delay(1);
	E_0;
	HAL_Delay(3); //Т.к. не проверяется бит готовности BF
}


//Отправить один символ на дисплей
void LCD_Send_Char (uint8_t data)
{
	RS_1;
	LCD_Send(data>>4);
	E_1;
	HAL_Delay(1);
	E_0;
	LCD_Send(data);
	E_1;
	HAL_Delay(1);
	E_0;
	HAL_Delay(3); //Т.к. не проверяется бит готовности BF
}


//Отправить строку на дисплей
void LCD_Send_String (uint8_t *str, uint8_t size)
{
	for (int i = 0; i < size; i++) {
		LCD_Send_Char(str[i]);
	}
}


//Установить курсор в указанную позицию
void LCD_Set_Cursor (uint8_t row, uint8_t col)
{
	uint8_t cursor_pos = 0x80;
	//Если строка = 2 установить курсор на 2-ю строку
	if (row == 2) {
		cursor_pos |= 0x40;
	} else { //в остальных случаях установить курсор на 1-ю строку
		cursor_pos |= 0x00;
	}

	//Если значение столбца не превышает 16, установить курсор в позицию col
	if (col < 16) {
		cursor_pos |= col;
	} else { //в остальных случаях установить курсор в начало строки
		cursor_pos |= 0x00;
	}
	LCD_Send_Command(cursor_pos);
}


//Очистить экран
void LCD_Clear (void)
{
	LCD_Send_Command (0x01); //очистить экран
	HAL_Delay(3);
}


//Инициализация дисплея
void LCD_Init (void)
{
	HAL_Delay(15);
	E_0;
	LCD_Send(0x03);		//1
	E_1; HAL_Delay(1);
	E_0; HAL_Delay(4);
	LCD_Send(0x03);		//2
	E_1; HAL_Delay(1);
	E_0; HAL_Delay(1);
	LCD_Send(0x03);		//3
	E_1; HAL_Delay(1);
	E_0; HAL_Delay(1);

	LCD_Send(0x02);
	E_1; HAL_Delay(1);
	E_0; HAL_Delay(3);

	LCD_Send_Command (0x28); //0x0010 1000 [5:0] 4-х разрядная шина данных, [4:1] две строки, [3:0] 5х8 точек
	HAL_Delay(3);

	LCD_Send_Command (0x08); //0x0000 1000 [3:0] выключить вывод изображения на экран
	HAL_Delay(3);

	LCD_Send_Command (0x01); //0x0000 0001 [1:1] очистить экран
	HAL_Delay(3);

	LCD_Send_Command (0x03); //0x0000 0011 [1:1] сдвиг содержимого экрана [2:1] влево
	HAL_Delay(3);

	LCD_Send_Command (0x0C); //0x0000 1110 [3:1] включить вывод изобажения на экран, [2:1] отобразить курсор в виде черты
	HAL_Delay(3);
}
