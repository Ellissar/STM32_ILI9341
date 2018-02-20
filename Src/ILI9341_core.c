/*
 * ILI9341.c
 *
 *  Created on: 11 февр. 2018 г.
 *      Author: ellissar
 *
 *		8080 MCU 16-bit bus interface I
 *
 *      Здесь находятся низкоуровневые функии для инициализации портов микроконтроллера,
 *      функции чтения/записи команд в/из дисплей/я, функция иницализации дисплея.
 *      Порядок подключения и работы с дисплеем описан в файле ILI9341.h
 *
 */


#include "ILI9341_core.h"


//--------------------------------------------------------------------
//Глобальные переменные
//--------------------------------------------------------------------

uint16_t ili9341_MIN_X = 0x00;
uint16_t ili9341_MIN_Y = 0x00;
uint16_t ili9341_MAX_X = 0x00;
uint16_t ili9341_MAX_Y = 0x00;


//--------------------------------------------------------------------
//Вспомогательные функции
//--------------------------------------------------------------------

//Настройка выводов, управляющих чтением/записью в дисплей
void ili9341_Init_Command_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_COMMANDS->BRR = (uint32_t)(CS_PIN|WR_PIN|RD_PIN|DC_PIN);

	GPIO_InitStruct.Pin = CS_PIN|WR_PIN|RD_PIN|DC_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIO_COMMANDS, &GPIO_InitStruct);
}


//Настройка порта для записи в дисплей
void ili9341_Set_Data_Pin_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_DATA->BRR = (uint32_t)GPIO_PIN_All;

	GPIO_InitStruct.Pin = GPIO_PIN_All;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIO_DATA, &GPIO_InitStruct);
}


//Настройка порта для чтения из дисплея
void ili9341_Set_Data_Pin_In(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_DATA->BRR = (uint32_t)GPIO_PIN_All;

	GPIO_InitStruct.Pin = GPIO_PIN_All;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIO_DATA, &GPIO_InitStruct);
}


//--------------------------------------------------------------------
//Основные функции
//--------------------------------------------------------------------

//Отправка команды/адреса регистра дисплею
void ili9341_Send_Cmd (uint8_t cmd)
{
	CS_LOW								//Активируем чип
	DC_LOW								//Будем отправлять адрес регистра
	RD_HIGH
	WR_LOW
	GPIO_DATA->ODR = (uint32_t)cmd;
    /*Если не будет работать необходимо попробовать добавить здесь задержку
     * около 5 МИКРОсекунд. При этом заметно уменьшится скорость обновления дисплея.
     * У меня прекрасно работает без задержки.*/
	WR_HIGH
	CS_HIGH								//Деактивируем чип
}


//Чтение из дисплея
uint8_t ili9341_Read_Data (void)
{
	uint8_t data = 0x00;
	ili9341_Set_Data_Pin_In();
	CS_LOW								//Активируем чип
	WR_HIGH
	DC_HIGH
	RD_LOW
	data = (uint8_t)GPIO_DATA->IDR;
    /*Если не будет работать необходимо попробовать добавить здесь задержку
     * около 5 МИКРОсекунд. При этом заметно уменьшится скорость обновления дисплея.
     * У меня прекрасно работает без задержки.*/
	RD_HIGH
	CS_HIGH								//Деактивируем чип
	ili9341_Set_Data_Pin_Out();
	return data;
}


//Запись 8/16 бит в дисплей
void ili9341_Write_Data (uint16_t data)
{
	CS_LOW //Активируем чип
	DC_HIGH //Будет отправлять параметры или данные
	RD_HIGH
	WR_LOW
	GPIO_DATA->ODR = (uint32_t)data;
    /*Если не будет работать необходимо попробовать добавить здесь задержку
     * около 5 МИКРОсекунд. При этом заметно уменьшится скорость обновления дисплея.
     * У меня прекрасно работает без задержки.*/
	WR_HIGH
	CS_HIGH //Деактивируем чип
}


//Разбивает 16 бит по 8 и отправляет на дисплей
void ili9341_Send_Data (uint16_t data)
{
	ili9341_Write_Data( data>>8 );	//Первым отправляем старший бит
	ili9341_Write_Data( data );		//Вторым отправляем младший бит
}


//Установка ориентации дисплея
void ili9341_Set_Orientation (uint8_t orient)
{
	ili9341_Send_Cmd(LCD_MAC);
	switch (orient)
	{
		case ORIENTATION_PORTRAIT: 			ili9341_Write_Data(ORIENTATION_PORTRAIT);
				break;
		case ORIENTATION_LANDSCAPE: 		ili9341_Write_Data(ORIENTATION_LANDSCAPE);
				break;
		case ORIENTATION_PORTRAIT_MIRROR: 	ili9341_Write_Data(ORIENTATION_PORTRAIT_MIRROR);
				break;
		case ORIENTATION_LANDSCAPE_MIRROR: 	ili9341_Write_Data(ORIENTATION_LANDSCAPE_MIRROR);
				break;
	}
	if (orient == 0 || orient == 2)
	{
		ili9341_MAX_X = 239;
		ili9341_MAX_Y = 319;
	}
	else
	{
		ili9341_MAX_X = 319;
		ili9341_MAX_Y = 239;
	}
}


//Инициализация дисплея
void ili9341_Init (void)
{
	//Настраиваем все выводы как выходы
	ili9341_Init_Command_Pin();
	ili9341_Set_Data_Pin_Out();

	ili9341_Send_Cmd(LCD_SWRESET);	//Software Reset
	HAL_Delay(1000);

	//Power Control 1
	ili9341_Send_Cmd(LCD_POWER1);	//задаём градацию серого цвета
	ili9341_Write_Data(0x25);

	//Power Control 2
	ili9341_Send_Cmd(LCD_POWER2);	//настройка step up преобразователя
	ili9341_Write_Data(0x01);

	//VCOM Control 1
	ili9341_Send_Cmd(LCD_VCOM1);	//контрастность определяется разностью VCOMH - VCOML = 5.2V
	ili9341_Write_Data(0x2B);		//VCOMH = 3.775
	ili9341_Write_Data(0x2B);		//VCOML = -1.425

	//VCOM Control 2
	ili9341_Send_Cmd(LCD_VCOM2);	//на Vcom по сути ШИМ, а тут мы задаем offset для него
	ili9341_Write_Data(0x06);		//VML=58 VMH=58

	//Memory Access Control
	ili9341_Set_Orientation(ORIENTATION_LANDSCAPE);	//выбираем ориентацию дисплея

	//COLMOD: Pixel Format Set
	ili9341_Send_Cmd(LCD_PIXEL_FORMAT);				//один пиксель будет кодироваться 16 битами
	ili9341_Write_Data(0x05);

	//Frame Rate Control
	ili9341_Send_Cmd(LCD_FRMCTR1);
	ili9341_Write_Data(0x00);
	ili9341_Write_Data(0x18);		//Frame Rate 79Hz

	//Display Function Control
	ili9341_Send_Cmd(LCD_DFC);
	ili9341_Write_Data(0x0A);
	ili9341_Write_Data(0x82);		//восьмой бит определяет нормальный цвет кристала белый - 1, черный - 0,
	ili9341_Write_Data(0x27);

	//COLMOD: Pixel Format Set
	ili9341_Send_Cmd(LCD_PIXEL_FORMAT);
	ili9341_Write_Data(0x55);

	//Sleep Out
	ili9341_Send_Cmd(LCD_SLEEP_OUT);

	HAL_Delay(120);

	//Display On
	ili9341_Send_Cmd(LCD_DISPLAY_ON);
}

