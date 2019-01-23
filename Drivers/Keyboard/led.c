#include "stdint.h"
#include "led.h"

/*--------------------------------------------------
Определение пинов
---------------------------------------------------*/

#define LED_Cancel_X	GPIO_X3_Pin
#define LED_Cancel_Y	GPIO_Y3_Pin

#define LED_Delay_X		GPIO_X2_Pin
#define LED_Delay_Y		GPIO_Y3_Pin

#define LED_1_X				GPIO_X1_Pin
#define LED_1_Y				GPIO_Y3_Pin

#define LED_2_X				GPIO_X1_Pin
#define LED_2_Y				GPIO_Y2_Pin

#define LED_3_X				GPIO_X2_Pin
#define LED_3_Y				GPIO_Y2_Pin

#define LED_4_X				GPIO_X3_Pin
#define LED_4_Y				GPIO_Y2_Pin

#define LED_5_X				GPIO_X3_Pin
#define LED_5_Y				GPIO_Y1_Pin

#define LED_Heating_X	GPIO_X2_Pin
#define LED_Heating_Y	GPIO_Y1_Pin

#define LED_Start_X		GPIO_X1_Pin
#define LED_Start_Y		GPIO_Y1_Pin

uint16_t LED_Mask_X[3];

/*--------------------------------------------------
функция включения/выключения/мигания светодиодов
----------------------------------------------------*/
void LED_Control(uint16_t led, uint8_t state)
{
// формирование маски для светодиодов для линии Y1
	uint16_t mask = 0;
	if(led & LED_Cancel)
	{
			mask |= LED_Cancel_X;
	};
	if(led & LED_Delay)
	{
			mask |= LED_Delay_X;
	};
	if(led & LED_1)
	{
			mask |= LED_1_X;
	};
	
	switch(state){
		case LED_On:
			LED_Mask_X[2] |= mask;
			break;
		case LED_Off:
			LED_Mask_X[2] &= (~mask);
			break;
		case LED_Toggle:
			LED_Mask_X[2] ^= mask;
			break;
	}
// формирование маски для светодиодов для линии Y2
	mask = 0;
	if(led & LED_2)
	{
			mask |= LED_2_X;
	};
	if(led & LED_3)
	{
			mask |= LED_3_X;
	};
	if(led & LED_4)
	{
			mask |= LED_4_X;
	};
	switch(state){
		case LED_On:
			LED_Mask_X[1] |= mask;
			break;
		case LED_Off:
			LED_Mask_X[1] &= (~mask);
			break;
		case LED_Toggle:
			LED_Mask_X[1] ^= mask;
			break;
	}
// формирование маски для светодиодов для линии Y3
	mask = 0;
	if(led & LED_5)
	{
			mask |= LED_5_X;
	};
	if(led & LED_Heating)
	{
			mask |= LED_Heating_X;
	};
	if(led & LED_Start)
	{
			mask |= LED_Start_X;
	};

	switch(state){
		case LED_On:
			LED_Mask_X[0] |= mask;
			break;
		case LED_Off:
			LED_Mask_X[0] &= (~mask);
			break;
		case LED_Toggle:
			LED_Mask_X[0] ^= mask;
			break;
	}
};

/*--------------------------------------------------
функция включения/выключения подсветки
----------------------------------------------------*/
void LED_Light(uint8_t state)
{
	switch(state){
		case LED_On:
			GPIO_Led_GPIO_Port->BSRR = GPIO_Led_Pin;
			break;
		case LED_Off:
			GPIO_Led_GPIO_Port->BRR = GPIO_Led_Pin;
			break;
		case LED_Toggle:
			GPIO_Led_GPIO_Port->ODR ^= GPIO_Led_Pin;
			break;
	};
};

/*--------------------------------------------------
функция настройки выводов GPIO 
----------------------------------------------------*/
void LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
	
  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = GPIO_Y1_Pin|GPIO_Y2_Pin|GPIO_Y3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

};

/******************************************************************/
