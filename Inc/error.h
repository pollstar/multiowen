/**
  ******************************************************************************
  * @file ERROR.h 
  * @author  pollstar
  * @version  V1.0.0
  * @date  29/02/2016
  * @brief  
  ******************************************************************************
  * @copy
  *
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __ERROR_H

#define __ERROR_H

#include "stm32f1xx.h"
#include <stdint.h>

#define ERROR_RTC_INIT			0
#define ERROR_RCC_OSC				1
#define ERROR_RCC_CLOCK			2
#define ERROR_RCC_PEREFCLK	3

/*--- Критические ошибки ---*/

#define ERROR_E1						0x81		// ошибка E1, обрыв или замыкание нижнего датчика
#define ERROR_E2						0x82   // ошибка E2, обрыв или низкая температура нижнего датчика
#define ERROR_E3						0x83		// ошибка E3, обрыв или замыкание верхнего датчика
#define ERROR_E4						0x84		// ошибка чтения показаний датчиков из АЦП

#define ERROR_LEN_BUF 50
extern uint8_t errorFlag;			// есть ошибки в буфере
extern uint8_t ErrorBuf[ERROR_LEN_BUF];

void Error_Handler(uint8_t err);

#endif /* __ERROR_H */

/************************END OF FILE****/

