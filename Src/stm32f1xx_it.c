/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include "power.h"
#include "led.h"



/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern RTC_HandleTypeDef hrtc;


/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  osSystickHandler();
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
*/
void TIM1_UP_TIM16_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim16);
	HAL_GPIO_TogglePin(GPIO_Buz1_GPIO_Port, GPIO_Buz1_Pin|GPIO_Buz2_Pin);
}

void TIM7_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim7);
}

/**
* @brief This function handles TIM1 trigger and commutation interrupts and TIM17 global interrupt.
*/

void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim17);
}

/**
* @brief This function handles EXTI line[9:5] interrupts.
* здесь идет приращение счеткика перехода фазы, если он остановлен, значить 
* нужно перейти в режим выключено
*/
void EXTI9_5_IRQHandler(void)
{
	PWR_Count++;
	PWR_On = true;
//	LED_Light(LED_Toggle);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

/**
* @brief This function handles RTC global interrupt.
*/
void RTC_IRQHandler(void)
{
  HAL_RTCEx_RTCIRQHandler(&hrtc);
	
}

/**
* @brief This function handles PVD interrupt through EXTI line 16.
*/
void PVD_IRQHandler(void)
{
	
	HAL_PWR_PVD_IRQHandler();
}

/**
* @brief This function handles ADC1 global interrupt.
*/
void ADC1_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
}

/*****************************END OF FILE****************************************/

