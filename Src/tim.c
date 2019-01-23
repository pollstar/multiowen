/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "led.h"
#include "lcd.h"
#include "power.h"
#include "keyboard.h"
#include <stdint.h>
#include "stm32f1xx.h"   // Device header

TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;
TIM_HandleTypeDef htim7;


/* TIM16 таймер, формирующий звук */
void MX_TIM16_Init(void)
{

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = SystemCoreClock/(80000);
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = (80000/(4023*2))-1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim16);


}
/* TIM16 deinit function */
void MX_TIM16_DeInit(void)
{

  HAL_TIM_Base_DeInit(&htim16);
}
/* TIM7 таймер опроса клавиатуры */
void MX_TIM7_Init(void)
{
//  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = SystemCoreClock/(40000);
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = (40000/(100*2))-1;
  HAL_TIM_Base_Init(&htim7);

	HAL_TIM_Base_Start_IT(&htim7);
}
/* TIM7 deinit function */
void MX_TIM7_DeInit(void)
{
  HAL_TIM_Base_DeInit(&htim7);

}
//static uint16_t clt17;
/* TIM17 таймер вывода изображения на дисплей */
void MX_TIM17_Init(uint16_t cl)
{
//	clt17 = cl;
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = HAL_RCC_GetPCLK1Freq()/(20000);
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = (20000/(cl*2))-1;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim17);

	HAL_TIM_Base_Start_IT(&htim17);
}


/* TIM17 deinit function */
void MX_TIM17_DeInit(void)
{
  HAL_TIM_Base_DeInit(&htim17);

}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM16)
  {
    /* Peripheral clock enable */
    __TIM16_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
  }
  else  if(htim_base->Instance==TIM7)
  {
    /* Peripheral clock enable */
    __TIM7_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM7_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
  }
  else  if(htim_base->Instance==TIM17)
	{
		    __TIM17_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
	};
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM16)
  {
    /* Peripheral clock disable */
    __TIM16_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

  }
  else if(htim_base->Instance==TIM7)
  {
    /* Peripheral clock disable */
    __TIM7_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM7_IRQn);

  }
  else if(htim_base->Instance==TIM17)
  {
    __TIM17_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM17_IRQn);
	}
} 

/*******************************************************************

callback функция обработки прерывания по событию обновления таймера

*******************************************************************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
	// обратока прерывания для проверки кнопок и вывода LED индикации 
	if(htim->Instance == TIM7){
		KEY_TIM_Handler();
	}
	// обратока прерывания для вывода LCD индикации 
	else if(htim->Instance == TIM17)
	{
		if(PWR_On)
			LCD_TIM_Handler();
		else
			LCD_TIM_HandlerMin();
	};
};
	

/*****************************END OF FILE****/
