#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "tim.h"

void BeepSetTone(uint16_t tone)
{
  htim16.Init.Period = (80000/(tone*2))-1;
//	htim16.Instance->ARR = (80000/(tone*2))-1;
  HAL_TIM_Base_Init(&htim16);
};

void Beep(void)
{
		BeepSetTone(2023);
		HAL_TIM_Base_Start_IT(&htim16);
		osDelay(30);
		BeepSetTone(3023);
		HAL_TIM_Base_Start_IT(&htim16);
		osDelay(40);
//		BeepSetTone(4023);
//		HAL_TIM_Base_Start_IT(&htim16);
//		osDelay(50);
		HAL_TIM_Base_Stop_IT(&htim16);
//		BeepSetTone(4023);
};

void BeepTime(uint16_t delay)
{
		BeepSetTone(523);
		HAL_TIM_Base_Start_IT(&htim16);
		osDelay(delay);
		HAL_TIM_Base_Stop_IT(&htim16);
};

/********************************************************
*   
*
*********************************************************/
