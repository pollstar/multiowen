/**
  ******************************************************************************
  * File Name          : power.h
  * Description        : Управление режымами питания и контроль
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __power_H
#define __power_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#define LCD_TIM_CLOCK_MIN  90
#define LCD_TIM_CLOCK_MAX  250
	 
/*---- счетчик контроля питания*/
extern uint32_t PWR_Count;
extern uint8_t PWR_On;
	 
	 
void PowerFull(void);
void PowerMin(void);
void PowerHandle(void);
	 
#ifdef __cplusplus
}
#endif
#endif /*__ power_H */


/*****************************END OF FILE****/
