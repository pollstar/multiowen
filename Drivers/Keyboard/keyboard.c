#include "keyboard.h"
#include "gpio.h"
#include "led.h"
#include <stdbool.h>

uint16_t KEY_Mask_X[3];

#define KEY_CANCEL_X				((uint16_t) 0x0400)
#define KEY_MENU_X					((uint16_t) 0x0200)
#define KEY_SELECT_X				((uint16_t) 0x0100)
#define KEY_DELAY_X					((uint16_t) 0x0100)
#define KEY_TEMP_X					((uint16_t) 0x0400)
#define KEY_TIME_X					((uint16_t) 0x0200)
#define KEY_START_X					((uint16_t) 0x0200)
#define KEY_TIMECOOKING_X		((uint16_t) 0x0100)


/************************************************
выдает ИСТИНА если нажата кнопка
***********************************************/
bool KEYPressed(void)
{
		if(KEY_Mask_X[0]|KEY_Mask_X[1]|KEY_Mask_X[2])
			return true;
		else
			return false;
};
/************************************************
выдает сборную маску нажатых кнопок
***********************************************/
uint16_t GetKeys(void)
{
	uint16_t result = 0;
	if(KEY_Mask_X[0] & KEY_START_X)
		result |= KEY_START;
	if(KEY_Mask_X[0] & KEY_TIMECOOKING_X)
		result |= KEY_TIMECOOKING;
	if(KEY_Mask_X[0] & KEY_TEMP_X)
		result |= KEY_TEMP;

	if(KEY_Mask_X[1] & KEY_TIME_X)
		result |= KEY_TIME;
	if(KEY_Mask_X[1] & KEY_DELAY_X)
		result |= KEY_DELAY;

	if(KEY_Mask_X[2] & KEY_SELECT_X)
		result |= KEY_SELECT;
	if(KEY_Mask_X[2] & KEY_CANCEL_X)
		result |= KEY_CANCEL;
	if(KEY_Mask_X[2] & KEY_MENU_X)
		result |= KEY_MENU;
	
	return result;
};


void KEY_TIM_Handler(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0,GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_LOW};
	static uint8_t linecount=0;  // указатель текущей обрабатываемой строки 

//	// Включаем подсветку, если надо
//	 HAL_GPIO_TogglePin(GPIO_Led_GPIO_Port, GPIO_Led_Pin);			
	
	// выключаем все
	GPIO_X1_GPIO_Port->BRR = LED_All_X;
	GPIO_Y1_GPIO_Port->BSRR = LED_All_Y;
	// проверяем текущую строку
	switch(linecount)
	{
		case 0:
			// считываем нажатые кнопки на линии Y1
			GPIO_InitStruct.Pin = KEY_COL_X_ALL;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIO_X1_GPIO_Port, &GPIO_InitStruct);
			GPIO_Y1_GPIO_Port->BRR = KEY_COL_Y_ALL;
			GPIO_Y1_GPIO_Port->BSRR = KEY_LINE_Y1;
	//				KEY_Mask_X[0] = KEY_COL_X_ALL;
			KEY_Mask_X[0] = (GPIO_X1_GPIO_Port->IDR) & KEY_COL_X_ALL;
			// зажигаем светодиоды
			GPIO_InitStruct.Pin = KEY_COL_X_ALL;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(GPIO_X1_GPIO_Port, &GPIO_InitStruct);
			GPIO_X1_GPIO_Port->BSRR = LED_Mask_X[linecount];
			GPIO_Y1_GPIO_Port->ODR ^= LED_All_Y;
			break;
		case 1:
			// считываем нажатые кнопки на линии Y1
			GPIO_InitStruct.Pin = KEY_COL_X_ALL;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIO_X1_GPIO_Port, &GPIO_InitStruct);
			GPIO_Y1_GPIO_Port->BRR = KEY_COL_Y_ALL;
			GPIO_Y2_GPIO_Port->BSRR = KEY_LINE_Y2;
			KEY_Mask_X[1] = GPIO_X1_GPIO_Port->IDR & KEY_COL_X_ALL;
			// зажигаем светодиоды
			GPIO_InitStruct.Pin = KEY_COL_X_ALL;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(GPIO_X1_GPIO_Port, &GPIO_InitStruct);
			GPIO_X1_GPIO_Port->BSRR = LED_Mask_X[linecount];
			GPIO_Y1_GPIO_Port->ODR ^= LED_All_Y;
			break;
		case 2:
			// считываем нажатые кнопки на линии Y3
			GPIO_InitStruct.Pin = KEY_COL_X_ALL;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIO_X1_GPIO_Port, &GPIO_InitStruct);
			GPIO_Y1_GPIO_Port->BRR = KEY_COL_Y_ALL;
			GPIO_Y3_GPIO_Port->BSRR = KEY_LINE_Y3;
			KEY_Mask_X[2] = GPIO_X1_GPIO_Port->IDR & KEY_COL_X_ALL;
			// зажигаем светодиоды
			GPIO_InitStruct.Pin = KEY_COL_X_ALL;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(GPIO_X1_GPIO_Port, &GPIO_InitStruct);
			GPIO_X1_GPIO_Port->BSRR = LED_Mask_X[linecount];
			GPIO_Y1_GPIO_Port->ODR ^= LED_All_Y;
			break;
	};
	if(++linecount > 2) linecount = 0;
};


/************************ END FILE **** keyboard.c *************************/
