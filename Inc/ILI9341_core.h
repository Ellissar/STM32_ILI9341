/*
 * ILI9341.h
 *
 *  Created on: 11 февр. 2018 г.
 *      Author: ellissar
 *
 *		8080 MCU 16-bit bus interface I
 *
 *		Для начала работы с экраном необходимо:
 *		1. Если используются выводы и порты, отличные от моих, то заменить их в define
 *			ниже в этом файле. (CS_PIN, DC_PIN, WR_PIN, RD_PIN, GPIO_COMMANDS, GPIO_DATA)
 *		2. Перед выводом однократно вызвать ункцию инициализации дисплея ili9341_Init()
 *		3. После этого можно использовать функции вывода графики.
 *
 */

#ifndef ILI9341_CORE_H_
#define ILI9341_CORE_H_


#include "stm32f3xx_hal.h"
#include "ILI9341_registers.h"


//--------------------------------------------------------------------
//Выводы и порты микроконтролера, используемые для управления дисплеем.
//Необходимо изменить выводы и порты на свои, используемые в проекте.
//--------------------------------------------------------------------
#define CS_PIN			ILI9341_CSX_Pin //PA1
#define DC_PIN			ILI9341_DCX_Pin //PA4
#define WR_PIN			ILI9341_WRX_Pin //PA2
#define RD_PIN			ILI9341_RDX_Pin //PA3

#define GPIO_COMMANDS	GPIOA 			/* Порт для управления дисплеем (чтение, запись).
										 * На этом порту находятся выводы (CS_PIN, DC_PIN, WR_PIN, RD_PIN). */
#define GPIO_DATA		GPIOD			//Потр для отправки команд и данных. Используются все 16 выводов.


//--------------------------------------------------------------------
/* Отказ от функции HAL_GPIO_WritePin в пользу прямого обращения к регистрам BSRR и BRR
 * показал трехкратный прирост скорости при заливке экрана одним цветом*/
#define CS_LOW			GPIO_COMMANDS->BRR = (uint32_t)CS_PIN;
#define CS_HIGH			GPIO_COMMANDS->BSRR = (uint32_t)CS_PIN;

#define DC_LOW			GPIO_COMMANDS->BRR = (uint32_t)DC_PIN;
#define DC_HIGH			GPIO_COMMANDS->BSRR = (uint32_t)DC_PIN;

#define WR_LOW			GPIO_COMMANDS->BRR = (uint32_t)WR_PIN;
#define WR_HIGH			GPIO_COMMANDS->BSRR = (uint32_t)WR_PIN;

#define RD_LOW			GPIO_COMMANDS->BRR = (uint32_t)RD_PIN;
#define RD_HIGH			GPIO_COMMANDS->BSRR = (uint32_t)RD_PIN;


//--------------------------------------------------------------------
//Глобальные переменные
//--------------------------------------------------------------------
extern uint16_t ili9341_MIN_X;
extern uint16_t ili9341_MIN_Y;
extern uint16_t ili9341_MAX_X;
extern uint16_t ili9341_MAX_Y;


//--------------------------------------------------------------------
//Вспомогательные функции
//--------------------------------------------------------------------
void ili9341_Init_Command_Pin(void);
void ili9341_Set_Data_Pin_Out(void);
void ili9341_Set_Data_Pin_In(void);


//--------------------------------------------------------------------
//Основные функции
//--------------------------------------------------------------------
void ili9341_Send_Cmd (uint8_t cmd);
uint8_t ili9341_Read_Data (void);
void ili9341_Write_Data (uint16_t data);
void ili9341_Send_Data (uint16_t data);
void ili9341_Set_Orientation (uint8_t orient);
void ili9341_Init (void);


#endif /* ILI9341_CORE_H_ */
