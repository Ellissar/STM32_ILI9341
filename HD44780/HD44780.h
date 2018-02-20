/*
 * HD44780.h
 *
 *  Created on: 2 февр. 2018 г.
 *      Author: ellissar
 */

#ifndef HD44780_H_
#define HD44780_H_

#include "stm32f3xx_hal.h"

//Изменить следующие 9 define на свои если необходимо
#define LCD_RS_PIN			LCD_RS_Pin	//PA10
#define LCD_E_PIN			LCD_E_Pin	//PA8
#define LCD_DB4_PIN			LCD_DB4_Pin	//PC6
#define LCD_DB5_PIN			LCD_DB5_Pin	//PC7
#define LCD_DB6_PIN			LCD_DB6_Pin	//PC8
#define LCD_DB7_PIN			LCD_DB7_Pin	//PC9
#define LCD_GPIO_PORT1		GPIOA
#define LCD_GPIO_PORT2		GPIOC


#define RS_1 HAL_GPIO_WritePin (LCD_GPIO_PORT1, LCD_RS_PIN, GPIO_PIN_SET)
#define RS_0 HAL_GPIO_WritePin (LCD_GPIO_PORT1, LCD_RS_PIN, GPIO_PIN_RESET)

#define E_1 HAL_GPIO_WritePin (LCD_GPIO_PORT1, LCD_E_PIN, GPIO_PIN_SET)
#define E_0 HAL_GPIO_WritePin (LCD_GPIO_PORT1, LCD_E_PIN, GPIO_PIN_RESET)

#define DB4_1 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB4_PIN, GPIO_PIN_SET)
#define DB4_0 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB4_PIN, GPIO_PIN_RESET)

#define DB5_1 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB5_PIN, GPIO_PIN_SET)
#define DB5_0 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB5_PIN, GPIO_PIN_RESET)

#define DB6_1 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB6_PIN, GPIO_PIN_SET)
#define DB6_0 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB6_PIN, GPIO_PIN_RESET)

#define DB7_1 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB7_PIN, GPIO_PIN_SET)
#define DB7_0 HAL_GPIO_WritePin (LCD_GPIO_PORT2, LCD_DB7_PIN, GPIO_PIN_RESET)

//Включение или выключение ножек контроллера в соответствии с младшими битами числа
void LCD_Send(uint8_t buf);

//Отправить инструкцию дислпею
void LCD_Send_Command (uint8_t command);

//Отправить один символ на дисплей
void LCD_Send_Char (uint8_t data);

//Отправить строку на дисплей
void LCD_Send_String (uint8_t *str, uint8_t size);

//Установить курсор в указанную позицию
void LCD_Set_Cursor (uint8_t row, uint8_t col);

//Очистить экран
void LCD_Clear (void);

//Инициализациия дисплея
void LCD_Init (void);


#endif /* HD44780_H_ */
