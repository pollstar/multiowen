/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM16_Init(void);
void MX_TIM17_Init(uint16_t cl);
void MX_TIM7_Init(void);
void MX_TIM16_DeInit(void);
void MX_TIM7_DeInit(void);
void MX_TIM17_DeInit(void);


#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */


/*****************************END OF FILE****/
