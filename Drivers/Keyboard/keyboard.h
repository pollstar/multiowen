/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __keyboard_H
#define __keyboard_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define KEY_NOTPRESSED	false
#define KEY_PRESSED 	!(KEY_NOTPRESSED)

/* --------  определяем  кнопки */
#define KEY_CANCEL				((uint16_t) 0x0001)
#define KEY_MENU					((uint16_t) 0x0002)
#define KEY_SELECT				((uint16_t) 0x0004)
#define KEY_DELAY					((uint16_t) 0x0008)
#define KEY_TEMP					((uint16_t) 0x0010)
#define KEY_TIME					((uint16_t) 0x0020)
#define KEY_START					((uint16_t) 0x0040)
#define KEY_TIMECOOKING		((uint16_t) 0x0080)

#define KEY_MASK_LINE 0x9800 
#define KEY_MASK_COL 0x0700 
#define KEY_LINE_Y1 GPIO_Y1_Pin 
#define KEY_LINE_Y2 GPIO_Y2_Pin 
#define KEY_LINE_Y3 GPIO_Y3_Pin 
#define KEY_COL_Y_ALL (KEY_LINE_Y1|KEY_LINE_Y2|KEY_LINE_Y3)

#define KEY_COL_X1 GPIO_X1_Pin
#define KEY_COL_X2 GPIO_X2_Pin
#define KEY_COL_X3 GPIO_X3_Pin
#define KEY_COL_X_ALL (KEY_COL_X1|KEY_COL_X2|KEY_COL_X3)

/* флаг нажатой клавиши */
extern uint8_t fPressKey;
// маски считанные кнопок
extern uint16_t KEY_Mask_X[3];

/************************************************
выдает ИСТИНА если нажата кнопка
***********************************************/
bool KEYPressed(void);
/************************************************
выдает сборную маску нажатых кнопок
***********************************************/
uint16_t GetKeys(void);

void KEY_TIM_Handler(void);

#ifdef __cplusplus
}
#endif
#endif /*__ keyboard_H */


/*****************************END OF FILE****/
