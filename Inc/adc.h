/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


extern ADC_HandleTypeDef hadc1;


void MX_ADC1_Init(void);
void MX_ADC1_DeInit(void);


#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */


/*****************************END OF FILE****/
