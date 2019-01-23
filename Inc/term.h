/**
  ******************************************************************************
  * File Name          : TERM.h
  * Description        : 
	
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __term_H
#define __term_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

typedef enum eTerm {TermDown=0, TermUp} teTerm;

extern uint16_t termADCValue[2];		// значение АЦП терморезистора

void TERM_Init(void);
void TERM_DeInit(void);
uint8_t TERM_GetTemp(teTerm t);
	 
#ifdef __cplusplus
}
#endif
#endif /*__ term_H */


/*****************************END OF FILE****/
