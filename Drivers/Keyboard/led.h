/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __led_H
#define __led_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "gpio.h"
	 
#define LED_All_X (GPIO_X1_Pin|GPIO_X2_Pin|GPIO_X3_Pin)
#define LED_All_Y (GPIO_Y1_Pin|GPIO_Y2_Pin|GPIO_Y3_Pin)

#define LED_Y1				GPIO_Y1_Pin
#define LED_Y2				GPIO_Y2_Pin
#define LED_Y3				GPIO_Y3_Pin

	 

#define LED_Cancel		((uint16_t)0x0001)
#define LED_Delay 		((uint16_t)0x0002)
#define LED_1					((uint16_t)0x0004)
#define LED_2					((uint16_t)0x0008)
#define LED_3					((uint16_t)0x0010)
#define LED_4					((uint16_t)0x0020)
#define LED_5					((uint16_t)0x0040)
#define LED_Heating		((uint16_t)0x0080)
#define LED_Start			((uint16_t)0x0100)

enum eLED_State{LED_On=1, LED_Off, LED_Toggle} ;
typedef enum eLED_State tLED_State;

// маски управления диодами
extern uint16_t LED_Mask_X[3];

/*--------------------------------------------------
функция включения/выключения/мигания светодиодов
----------------------------------------------------*/
void LED_Control(uint16_t led, uint8_t state);
/*--------------------------------------------------
функция включения/выключения подсветки
----------------------------------------------------*/
void LED_Light(uint8_t state);
void LED_GPIO_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ led_H */


/*****************************END OF FILE****/
