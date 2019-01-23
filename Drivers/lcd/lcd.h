/**
  ******************************************************************************
  * @file lcd.h 
  * @author  pollstar
  * @version  V1.0.0
  * @date  21/02/2016
  * @brief  
  ******************************************************************************
  * @copy
  *
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __LCD_H

#define __LCD_H

#include "stm32f1xx.h"

/* LCD interrupt priority configuration ------------------------------------- */
#define LCD_Priority_Value       1

#define CHAR_h					10
#define CHAR_SPACE			11
#define CHAR_E					12


/* Common lines configuration ----------------------------------------------- */
#define CommonLines_Port  GPIO_COM_GPIO_Port /* Port where the 4 common lines are connected */

/* Segment lines configuration ---------------------------------------------- */
/* Port where the Low Quarter digits segment lines are connected */
#define SegmentsLines_LQD_Port 	GPIO_LCD1_GPIO_Port

/* Port where the High Quarter digits segment lines are connected */
#define SegmentsLines_HQD_Port   GPIO_LCD8_GPIO_Port


/* Parameters not modifiable by the user */
#define LCDContrastValue          80  /* LCD Contrast value in percent from 0 to 100 */
#define PulseValueForContrast     ((LCDContrastValue * PeriodValue) / 100)
#define DeadTimeValue             (PeriodValue - PulseValueForContrast)
#define PulseValueForContrastLow  (PulseValueForContrast & (uint32_t)0x0000FFFF)
#define PulseValueForContrastHigh ((PulseValueForContrast & (uint32_t)0xFFFF0000) >> 16)
#define DeadTimeValueLow          (DeadTimeValue & (uint32_t)0x0000FFFF)
#define DeadTimeValueHigh         ((DeadTimeValue & (uint32_t)0xFFFF0000) >> 16)

/*------ GPIOs Masks definitions (section not modifiable by the user) --------*/
 #define PinsConfig         CRH
 #define ALL_COMMON_LINES  (uint16_t)(0xF000)

#define ALL_COMMON_LINES_PP   (0x33330000)

#define ALL_SEGMENT_LINES      0x00FF
//#define ALL_SEGMENT_LINES_PP   0x00003333

/*------- Определяем индикацию программ ------ */
enum eLCD_IND
{
	LCD_IND_TERM = 0,\
	LCD_IND_RIS,\
	LCD_IND_PAR,\
	LCD_IND_VIPECHKA,\
	LCD_IND_CLOCK,\
	LCD_IND_DELAY,\
	LCD_IND_GRECHKA,\
	LCD_IND_TOMLENIE,\
	LCD_IND_RUCHNOY,\
	LCD_IND_MILK,\
	LCD_IND_JARKA,\
	LCD_IND_TUSHENIE,\
	LCD_IND_RAZOGREV,\
	LCD_IND_YOGURT,\
	LCD_IND_OVOCH,\
	LCD_IND_RIBA,\
	LCD_IND_MIASO,\
	LCD_IND_POINTS,\
	LCD_IND_OFF = 255
};

typedef enum eLCD_IND tLCD_IND;

enum eLCD_IND_State{IND_On=1, IND_Off, IND_Toggle, IND_Blink} ;
typedef enum eLCD_IND_State tLCD_IND_State;

void LCD_GPIO_Init(void);
//void LCD_GPIO_Coms_Masks_Init(void);
void LCD_WriteMinutes(uint8_t minute);
void LCD_WriteHour(uint8_t hour);
void LCD_WriteTime(uint8_t hour, uint8_t minute);
void LCD_WriteTimeHeating(uint8_t hour);
void LCD_WriteIndicator(tLCD_IND ind, tLCD_IND_State state);
void LCD_WriteChar(uint8_t c, uint8_t num);
void LCD_WriteTemp(uint8_t);
void LCD_TIM_Handler(void);
void LCD_TIM_HandlerMin(void);
void LCD_Off(void);
void LCD_WriteError(uint8_t error);
void LCD_IndicatorOn(void);
void LCD_IndicatorOff(void);
void LCD_IndicatorBlink(tLCD_IND ind);

#endif /* __LCD_H */

/************************END OF FILE****/

