#include "stm32f1xx.h"
#include "lcd.h"
#include "led.h"
#include <stdbool.h>
#include "rtc.h"
#include "power.h"
#include "programms.h"
#include "error.h"

/**************************************************************
сегменты для вывода индикации времени
   A1						A2					A3      		A4
	 ---  			 ---				 ---					---
B1|   |C1   B2|   |C2   B3|   |C3    B4|   |C4
D1 ---			D2 ---			D3 ---			 D4 ---
E1|		|F1		E2|		|F2		E3|		|E3		 E4|	 |D4
	 ---				 ---				 ---					---
	 G1						G2					G3					G4
*****************************************************************/
#define LCD_SEG_TIME_A1		18
#define LCD_SEG_TIME_B1		19
#define LCD_SEG_TIME_C1		20
#define LCD_SEG_TIME_D1		21
#define LCD_SEG_TIME_E1		22
#define LCD_SEG_TIME_F1		23
#define LCD_SEG_TIME_G1		24

#define LCD_SEG_TIME_A2		25
#define LCD_SEG_TIME_B2		26
#define LCD_SEG_TIME_C2		27
#define LCD_SEG_TIME_D2		28
#define LCD_SEG_TIME_E2		29
#define LCD_SEG_TIME_F2		30
#define LCD_SEG_TIME_G2		31

#define LCD_SEG_TIME_A3		32
#define LCD_SEG_TIME_B3		33
#define LCD_SEG_TIME_C3		34
#define LCD_SEG_TIME_D3		35
#define LCD_SEG_TIME_E3		36
#define LCD_SEG_TIME_F3		37
#define LCD_SEG_TIME_G3		38

#define LCD_SEG_TIME_A4		39
#define LCD_SEG_TIME_B4		40
#define LCD_SEG_TIME_C4		41
#define LCD_SEG_TIME_D4		42
#define LCD_SEG_TIME_E4		43
#define LCD_SEG_TIME_F4		44
#define LCD_SEG_TIME_G4		45

/**************************************************************
сегменты для вывода индикации температуры 
**************************************************************/
#define LCD_SEG_TEMP_A1		46

#define LCD_SEG_TEMP_A2		47
#define LCD_SEG_TEMP_B2		48
#define LCD_SEG_TEMP_C2		49
#define LCD_SEG_TEMP_D2		50
#define LCD_SEG_TEMP_E2		51
#define LCD_SEG_TEMP_F2		52
#define LCD_SEG_TEMP_G2		53

#define LCD_SEG_TEMP_A3		54
#define LCD_SEG_TEMP_B3	  55
#define LCD_SEG_TEMP_C3		56
#define LCD_SEG_TEMP_D3		57
#define LCD_SEG_TEMP_E3		58
#define LCD_SEG_TEMP_F3		59
#define LCD_SEG_TEMP_G3		60


const  uint16_t  CommonLine[4]={
                             GPIO_COM1_Pin,
                             GPIO_COM2_Pin,
                             GPIO_COM3_Pin,
                             GPIO_COM4_Pin};

/* GPIOs to be configured to VDD/2 */
uint32_t  CommonLine_VDD_2[4]={ 
                           0xF000FFFF,
                           0x0F00FFFF,
                           0x00F0FFFF,
                           0x000FFFF};
/* GPIOs to be configured to Output PP */
uint32_t  CommonLine_OUT_PP[4]={
                            0x30000000,
                            0x03000000,
                            0x00300000,
                            0x00030000
                           };

#define LCD_UM_IND	67
#define COM1 0
#define COM2 1
#define COM3 2
#define COM4 3
/* массив флагов и мест для элементов индикации*/													 
struct tLCD_Flag{
	uint16_t 	line;	// пин  индикатора
	GPIO_TypeDef *port; // порт для пина индикатора
	uint16_t		com;	// флаг знакоместа
}LCD_Flag[LCD_UM_IND]={\
							{GPIO_LCD4_Pin,GPIO_LCD4_GPIO_Port, COM1}, 	//LCD_IND_TERM			0
							{GPIO_LCD6_Pin, GPIO_LCD6_GPIO_Port, COM1}, 		//LCD_IND_RIS 			1
							{GPIO_LCD7_Pin,GPIO_LCD7_GPIO_Port, COM1}, 		//LCD_IND_PAR				2
							{GPIO_LCD8_Pin,GPIO_LCD8_GPIO_Port, COM1}, 		//LCD_IND_VIPECHKA	3
							{GPIO_LCD5_Pin,GPIO_LCD5_GPIO_Port, COM1}, 		//LCD_IND_CLOCK			4
							{GPIO_LCD5_Pin,GPIO_LCD5_GPIO_Port, COM2}, 		//LCD_IND_DELAY			5 *
							{GPIO_LCD6_Pin,GPIO_LCD6_GPIO_Port, COM2}, 		//LCD_IND_GRECHKA		6 *
							{GPIO_LCD7_Pin,GPIO_LCD7_GPIO_Port, COM3}, 		//LCD_IND_TOMLENIE	7
							{GPIO_LCD8_Pin,GPIO_LCD8_GPIO_Port, COM2}, 		//LCD_IND_RUCHNOY		8 *
							{GPIO_LCD6_Pin,GPIO_LCD6_GPIO_Port, COM3}, 		//LCD_IND_MILK			9
							{GPIO_LCD7_Pin,GPIO_LCD7_GPIO_Port, COM4}, 		//LCD_IND_JARKA			10
							{GPIO_LCD8_Pin,GPIO_LCD8_GPIO_Port, COM3}, 		//LCD_IND_TUSHENIE	11
							{GPIO_LCD6_Pin,GPIO_LCD6_GPIO_Port, COM4}, 		//LCD_IND_RAZOGREV	12
							{GPIO_LCD8_Pin,GPIO_LCD8_GPIO_Port, COM4}, 		//LCD_IND_YOGURT		13
							{GPIO_LCD10_Pin,GPIO_LCD10_GPIO_Port, COM1}, 	//LCD_IND_OVOCH			14
							{GPIO_LCD14_Pin,GPIO_LCD14_GPIO_Port, COM1}, 	//LCD_IND_RIBA			15
							{GPIO_LCD16_Pin,GPIO_LCD16_GPIO_Port, COM1}, 	//LCD_IND_MIASO		16
							{GPIO_LCD12_Pin,GPIO_LCD12_GPIO_Port, COM1}, 	//LCD_IND_POINTS	17

							{GPIO_LCD9_Pin,GPIO_LCD9_GPIO_Port, COM1}, 		//LCD_SEG_TIME_A1		18
							{GPIO_LCD9_Pin,GPIO_LCD9_GPIO_Port, COM2}, 		//LCD_SEG_TIME_B1		19
							{GPIO_LCD10_Pin,GPIO_LCD10_GPIO_Port, COM2}, 	//LCD_SEG_TIME_C1		20
							{GPIO_LCD10_Pin,GPIO_LCD10_GPIO_Port, COM3}, 	//LCD_SEG_TIME_D1		21
							{GPIO_LCD9_Pin,GPIO_LCD9_GPIO_Port, COM3}, 		//LCD_SEG_TIME_E1		22
							{GPIO_LCD10_Pin,GPIO_LCD10_GPIO_Port, COM4}, 	//LCD_SEG_TIME_F1		23
							{GPIO_LCD9_Pin,GPIO_LCD9_GPIO_Port, COM4}, 		//LCD_SEG_TIME_G1		24

							{GPIO_LCD11_Pin,GPIO_LCD11_GPIO_Port, COM1}, 		//LCD_SEG_TIME_A2		25
							{GPIO_LCD11_Pin,GPIO_LCD11_GPIO_Port, COM2}, 		//LCD_SEG_TIME_B2		26
							{GPIO_LCD12_Pin,GPIO_LCD12_GPIO_Port, COM2}, 		//LCD_SEG_TIME_C2		27
							{GPIO_LCD12_Pin,GPIO_LCD12_GPIO_Port, COM3}, 		//LCD_SEG_TIME_D2		28
							{GPIO_LCD11_Pin,GPIO_LCD11_GPIO_Port, COM3}, 		//LCD_SEG_TIME_E2		29
							{GPIO_LCD12_Pin,GPIO_LCD12_GPIO_Port, COM4}, 		//LCD_SEG_TIME_F2		30
							{GPIO_LCD11_Pin,GPIO_LCD11_GPIO_Port, COM4}, 		//LCD_SEG_TIME_G2		31
				
							{GPIO_LCD13_Pin,GPIO_LCD13_GPIO_Port, COM1}, 		//LCD_SEG_TIME_A3		32
							{GPIO_LCD13_Pin,GPIO_LCD13_GPIO_Port, COM2}, 		//LCD_SEG_TIME_B3		33
							{GPIO_LCD14_Pin,GPIO_LCD14_GPIO_Port, COM2}, 		//LCD_SEG_TIME_C3		34
							{GPIO_LCD14_Pin,GPIO_LCD14_GPIO_Port, COM3}, 		//LCD_SEG_TIME_D3		35
							{GPIO_LCD13_Pin,GPIO_LCD13_GPIO_Port, COM3}, 		//LCD_SEG_TIME_E3		36
							{GPIO_LCD14_Pin,GPIO_LCD14_GPIO_Port, COM4}, 		//LCD_SEG_TIME_F3		37
							{GPIO_LCD13_Pin,GPIO_LCD13_GPIO_Port, COM4}, 		//LCD_SEG_TIME_G3		38
				
							{GPIO_LCD15_Pin,GPIO_LCD15_GPIO_Port, COM1}, 		//LCD_SEG_TIME_A4		39
							{GPIO_LCD15_Pin,GPIO_LCD15_GPIO_Port, COM2}, 		//LCD_SEG_TIME_B4		40
							{GPIO_LCD16_Pin,GPIO_LCD16_GPIO_Port, COM2}, 		//LCD_SEG_TIME_C4		41
							{GPIO_LCD16_Pin,GPIO_LCD16_GPIO_Port, COM3}, 		//LCD_SEG_TIME_D4		42
							{GPIO_LCD15_Pin,GPIO_LCD15_GPIO_Port, COM3}, 		//LCD_SEG_TIME_E4		43
							{GPIO_LCD16_Pin,GPIO_LCD16_GPIO_Port, COM4}, 		//LCD_SEG_TIME_F4		44
							{GPIO_LCD15_Pin,GPIO_LCD15_GPIO_Port, COM4}, 		//LCD_SEG_TIME_G4		45
				

							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A1		46
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A1		46
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A1		46
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A1		46
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A1		46
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM2}, 	//LCD_SEG_TEMP_A1		46
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A1		46
							
							{GPIO_LCD1_Pin,GPIO_LCD1_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A2		47
							{GPIO_LCD1_Pin,GPIO_LCD1_GPIO_Port, COM2}, 	//LCD_SEG_TEMP_B2		48
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM2}, 	//LCD_SEG_TEMP_C2		49
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM3}, 	//LCD_SEG_TEMP_D2		50
							{GPIO_LCD1_Pin,GPIO_LCD1_GPIO_Port, COM3}, 	//LCD_SEG_TEMP_E2		51
							{GPIO_LCD2_Pin,GPIO_LCD2_GPIO_Port, COM4}, 	//LCD_SEG_TEMP_F2		52
							{GPIO_LCD1_Pin,GPIO_LCD1_GPIO_Port, COM4}, 	//LCD_SEG_TEMP_G2		53

							{GPIO_LCD3_Pin,GPIO_LCD3_GPIO_Port, COM1}, 	//LCD_SEG_TEMP_A3		54
							{GPIO_LCD3_Pin,GPIO_LCD3_GPIO_Port, COM2}, 	//LCD_SEG_TEMP_B3	  55
							{GPIO_LCD4_Pin,GPIO_LCD4_GPIO_Port, COM2}, 	//LCD_SEG_TEMP_C3		56
							{GPIO_LCD4_Pin,GPIO_LCD4_GPIO_Port, COM3}, 	//LCD_SEG_TEMP_D3		57
							{GPIO_LCD3_Pin,GPIO_LCD3_GPIO_Port, COM3}, 	//LCD_SEG_TEMP_E3		58
							{GPIO_LCD4_Pin,GPIO_LCD4_GPIO_Port, COM4}, 	//LCD_SEG_TEMP_F3		59
							{GPIO_LCD3_Pin,GPIO_LCD3_GPIO_Port, COM4}}; 	//LCD_SEG_TEMP_G3		60
							
#define DIGIT1_TIME 18
#define DIGIT2_TIME 25
#define DIGIT3_TIME 32
#define DIGIT4_TIME 19
							
#define SegA 1
#define SegB 2							
#define SegC 3							
#define SegD 4							
#define SegE 5							
#define SegF 6							
#define SegG 7							

uint8_t LCD_Char[13][7]=
							{{1,1,1,0,1,1,1},		// 0
							{0,0,1,0,0,1,0},		// 1
							{1,0,1,1,1,0,1},		// 2
							{1,0,1,1,0,1,1},		// 3
							{0,1,1,1,0,1,0},		// 4
							{1,1,0,1,0,1,1},		// 5
							{1,1,0,1,1,1,1},		// 6
							{1,0,1,0,0,1,0},		// 7
							{1,1,1,1,1,1,1},		// 8
							{1,1,1,1,0,1,1},		// 9
							{0,1,0,1,1,1,0},		// h
							{0,0,0,0,0,0,0},		// Space
							{1,1,0,1,1,0,1}};		// E

							/* Global variables definition and initialization ----------------------------*/
uint16_t  SegmentsValues_Lower_Quarter_Digits[4]; /* LCD frame buffer for low 
                                               quarter digits (digits 1 -> 4) */
uint16_t  SegmentsValues_Higher_Quarter_Digits[4]; /* LCD frame buffer for high 
                                                quarter digits (digits 5 -> 8) */
uint16_t  digit[4];     /* Digit frame buffer */


							
							
/**
* @brief  Включение/отключение индикатора на ЖК
* @param ind: индикатор
				 state: состояние
* @retval : None.
*/
void LCD_WriteIndicator(tLCD_IND ind, tLCD_IND_State state)
{
	if(LCD_Flag[ind].port == SegmentsLines_LQD_Port)
	{
		if(state == IND_On)
			SegmentsValues_Lower_Quarter_Digits[LCD_Flag[ind].com] |= LCD_Flag[ind].line;
		else if(state == IND_Off)
			SegmentsValues_Lower_Quarter_Digits[LCD_Flag[ind].com] &= (~LCD_Flag[ind].line);
		else
			SegmentsValues_Lower_Quarter_Digits[LCD_Flag[ind].com] ^= LCD_Flag[ind].line;
	}
	else if(LCD_Flag[ind].port == SegmentsLines_HQD_Port)
	{
		if(state == IND_On)
			SegmentsValues_Higher_Quarter_Digits[LCD_Flag[ind].com] |=LCD_Flag[ind].line;
		else if(state == IND_Off)
			SegmentsValues_Higher_Quarter_Digits[LCD_Flag[ind].com] &= (~LCD_Flag[ind].line);
		else
			SegmentsValues_Higher_Quarter_Digits[LCD_Flag[ind].com] ^= LCD_Flag[ind].line;
	};
};

/**
* @brief  Запись символа по знакоместу в буфер LCD
* @param minutes: показания минут
* @retval : None.
*/
void LCD_WriteChar(uint8_t c, uint8_t num)
{
	if((c < 12) && (num < 7)) // проверка параметров
	{
		uint16_t lcddatapos = DIGIT1_TIME + num*7;  // устанавливаем позицию чтения знака в знакоместо
		for(uint8_t i=0; i<7; i++)
		{
			if(LCD_Char[c][i])
			{
				if(LCD_Flag[i+lcddatapos].port == SegmentsLines_LQD_Port)
				{
						SegmentsValues_Lower_Quarter_Digits[LCD_Flag[i+lcddatapos].com] |= LCD_Flag[i+lcddatapos].line;
				}
				else if(LCD_Flag[i+lcddatapos].port == SegmentsLines_HQD_Port)
				{
						SegmentsValues_Higher_Quarter_Digits[LCD_Flag[i+lcddatapos].com] |=LCD_Flag[i+lcddatapos].line;
				};
			}
			else
			{
				if(LCD_Flag[i+lcddatapos].port == SegmentsLines_LQD_Port)
				{
						SegmentsValues_Lower_Quarter_Digits[LCD_Flag[i+lcddatapos].com] &= (~LCD_Flag[i+lcddatapos].line);
				}
				else if(LCD_Flag[i+lcddatapos].port == SegmentsLines_HQD_Port)
				{
						SegmentsValues_Higher_Quarter_Digits[LCD_Flag[i+lcddatapos].com] &= (~LCD_Flag[i+lcddatapos].line);
				};
			};
		};
	};
};

/**
* @brief  Запись показаний минут в буфер LCD
* @param minutes: показания минут
* @retval : None.
*/
void LCD_WriteMinutes(uint8_t minute)
{
	LCD_WriteChar(minute/10, 2);
	LCD_WriteChar(minute%10, 3);
};

/**
* @brief  Запись показаний часов в буфер LCD
* @param hour: показания часов.
* @retval : None.
*/
void LCD_WriteHour(uint8_t hour)
{
	if(hour > 9)
		LCD_WriteChar(hour/10, 0);
	else
		LCD_WriteChar(11, 0);
	LCD_WriteChar(hour%10, 1);
};

/**
* @brief  Запись показаний часов и минут в буфер LCD
* @param hour: показания часов.
* @param minutes: показания минут
* @retval : None.
*/
void LCD_WriteTime(uint8_t hour, uint8_t minute)
{
	LCD_WriteMinutes(minute);
	LCD_WriteHour(hour);
//	LCD_WriteIndicator(LCD_IND_POINTS, IND_On);
};

/**
* @brief  Запись показаний термометра в буфер LCD
* @param hour: показания часов.
* @param minutes: показания минут
* @retval : None.
*/
void LCD_WriteTemp(uint8_t temp)
{
	if(temp<200)
	{
		if(temp>99)
			LCD_WriteChar(2,4);
		else
			LCD_WriteChar(11,4);
		uint8_t tmp = (temp-((temp/100)*100));
		LCD_WriteChar((tmp/10),5);
		LCD_WriteChar((tmp%10),6);
//		LCD_WriteIndicator(LCD_IND_TERM, IND_On);
	}
};

/**
* @brief  Вывод ошибки с номером
* @param error: номер ошибки.
* @retval : None.
*/
void LCD_WriteError(uint8_t error)
{
		LCD_WriteChar(CHAR_E,1);
		LCD_WriteChar((error/10),2);
		LCD_WriteChar((error%10),3);
		LCD_WriteIndicator(LCD_IND_POINTS, IND_Off);
};

/**
  * @brief  Initialize GPIOs to set segments lines and common line
  *   values to 0.
  * @param ne.
  * @retval : None.
  */

void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
  /* Configure all common lines on CommonLines port pins as Out_PP */    
  GPIO_InitStruct.Pin = GPIO_COM1_Pin|GPIO_COM2_Pin|GPIO_COM3_Pin|GPIO_COM4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIO_Buz1_GPIO_Port, &GPIO_InitStruct);

  /* All common lines = 0 */
  CommonLines_Port->BRR = ALL_COMMON_LINES;
    
  /* Configure all segment lines of digits as Out_PP */ 
  GPIO_InitStruct.Pin = ALL_SEGMENT_LINES;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(SegmentsLines_LQD_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(SegmentsLines_HQD_Port, &GPIO_InitStruct);

	/* All segment lines = 0 ( all SegmentsLines_LQD_Port pins = 0 ) */
  HAL_GPIO_WritePin(SegmentsLines_LQD_Port, ALL_SEGMENT_LINES, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SegmentsLines_HQD_Port, ALL_SEGMENT_LINES, GPIO_PIN_RESET);

}

extern uint16_t SegmentsValues_Lower_Quarter_Digits[4];
extern uint16_t SegmentsValues_Higher_Quarter_Digits[4];
extern const uint16_t CommonLine[4];
extern uint32_t CommonLine_OUT_PP[4];
extern uint32_t CommonLine_VDD_2[4];
uint32_t lcdcr=0;
__IO uint32_t LCDPowerOff=0;

static bool EnableTask1 = true;
static bool EnableTask2_3 = false;
//static uint32_t pwrcnt = 0;

extern TIM_HandleTypeDef htim17;

void LCD_TIM_Handler(void)
{
	
	LED_Light(LED_Toggle);	
//  period++;
//	if(!((period)%250))
//	{
//		RTC_TimeTypeDef time;
//		HAL_RTC_GetTime(&hrtc, &time, FORMAT_BIN);
//		LCD_WriteTime(time.Hours, time.Minutes);
//		LCD_WriteIndicator(LCD_IND_POINTS, IND_Toggle);
////		osDelay(500);
//	};

//	if((period & 7) == 0)
//	{
//		if(pwrcnt != PWR_Count)
//			PWR_On = 1;
//		else
//			PWR_On = 0;
//		pwrcnt = PWR_Count;
//	}
	 /* Sequence of Tasks:
	 Task1 -> Task2 -> Task1 -> Task3 -> Task1 -> Task2 -> Task1 -> Task3 -> ...
	
	 * Task 1: all Common Lines and all Segment Lines are in push-pull and are set to 0.
	 * Task 2: - Segment_lines[lcdcr] to be turned on are loaded with the value 1 otherwise 0.
						 - Common_line[lcdcr] is set to low. 
						 - Other Common lines are set to Vdd/2.
	 * Task 3: - Segment_lines[lcdcr] values are inverted.
						 - Common_line[lcdcr] is set to high.
						 - Other Common lines are set to Vdd/2.
	*/ 
 
		 
	if(EnableTask1)					/*------------- Task 1 -------------*/
	{
	 /* All Segment lines = 0 ( all SegmentsLines_LQD_Port pins = 0 ) */
		 SegmentsLines_LQD_Port->BRR = (uint16_t)ALL_SEGMENT_LINES; 
		 SegmentsLines_HQD_Port->BRR = (uint16_t)ALL_SEGMENT_LINES;

		/* All Common lines = 0 */
		 CommonLines_Port->BRR = ALL_COMMON_LINES;				
		
		 /* Configure all Common lines on CommonLines port pins as Out_PP */    
		 CommonLines_Port->CRH &= 0x0000FFFF;
		 CommonLines_Port->CRH |= ALL_COMMON_LINES_PP;
			
		 if(EnableTask2_3) /* If Task 2 has been executed previously (before Task 1) then prepare */
		 {                         /* the RTC interrupt to execute Task 3 in the next interrupt */
			 /* Next interrupt will execute Task 3 and avoid it to execute Task 2 */
			 EnableTask2_3 = false;
		 }
		 else  /* If Task 3 has been executed previously (before Task 1) then prepare */
		 {     /* the RTC interrupt to execute Task 2 in the next interrupt */
			 /* Next interrupt will execute Task 2 and avoid it to execute Task 3 */
			 EnableTask2_3 = true; 
		 }
		 
		/* Avoid next interrupt to execute Task 1, so the interrupt will execute Task 2 or Task 3 */ 
		EnableTask1 = false;
	}
	else
	{ 
		if(EnableTask2_3)  /*------------- Task 2 --------------*/
		{
		 /* Segment_lines[lcdcr] to be turned on are loaded with the value 1 otherwise 0 */
			SegmentsLines_LQD_Port->BSRR = SegmentsValues_Lower_Quarter_Digits[lcdcr];  
			SegmentsLines_HQD_Port->BSRR = SegmentsValues_Higher_Quarter_Digits[lcdcr];
				 
			/* Common_line[lcdcr] is set to low */
			CommonLines_Port->BRR = CommonLine[lcdcr];

			/* Other Common lines set to Vdd/2 */
			CommonLines_Port->CRH &= CommonLine_VDD_2[lcdcr];
				 
			/* Set Common_line[lcdcr] out push pull */
			CommonLines_Port->CRH |= CommonLine_OUT_PP[lcdcr];
				 
			/* Установить в 1 */
//					HAL_GPIO_WritePin(CommonLines_Port,(uint16_t)(~(CommonLine[lcdcr]|0x0FFF)),GPIO_PIN_SET);
				// CommonLines_Port->BSRR = 0x0000F000 & (~CommonLine[lcdcr]);
//					 CommonLines_Port->BSRR = ~((uint32_t)CommonLine[lcdcr] | 0xFFFF0FFF);
				 
				 /* Next interrupt will execute Task 1 */
			EnableTask1 = true;
		}
		else                 /*------------- Task 3 -------------*/
		{
			 /* Segment_lines[lcdcr] value		s are inverted */ 
			 SegmentsLines_LQD_Port->BSRR = 
									((uint32_t)(~SegmentsValues_Lower_Quarter_Digits[lcdcr])&0x000000FF);  
			 SegmentsLines_HQD_Port->BSRR = 
									((uint32_t)(~SegmentsValues_Higher_Quarter_Digits[lcdcr])&0x000000FF);

			/* Common_line[lcdcr] is set to high */
			 CommonLines_Port->BSRR = CommonLine[lcdcr];
			 
			 /* Other Common lines set to Vdd/2 */
			 CommonLines_Port->CRH &= CommonLine_VDD_2[lcdcr];

			/* Other Common lines out push pull */
			 CommonLines_Port->CRH |= CommonLine_OUT_PP[lcdcr];
			 
			/* Установить в 0 */
//					 CommonLines_Port->BRR = ~((uint32_t)CommonLine[lcdcr] | 0xFFFF0FFF);
//				HAL_GPIO_WritePin(CommonLines_Port,(uint16_t)(~(CommonLine[lcdcr]|0x0FFF)),GPIO_PIN_RESET);

			//					 CommonLines_Port->BRR = (ALL_COMMON_LINES & (~CommonLine[lcdcr]));

			/* Next interrupt will execute Task 1 */
			 EnableTask1 = true;  

			lcdcr++;
			if(lcdcr>3)
			{
				lcdcr = 0;
			}
		}
	}
}	
	
void LCD_TIM_HandlerMin(void)
{
	static uint8_t errorcount = 0;
	static uint32_t period=0;

	if(!((period++)%LCD_TIM_CLOCK_MIN))
	{
		RTC_TimeTypeDef time;
		HAL_RTC_GetTime(&hrtc, &time, FORMAT_BIN);
		LCD_WriteTime(time.Hours, time.Minutes);
		LCD_WriteIndicator(LCD_IND_POINTS, IND_Toggle);//		LCD_WriteTemp(time.Seconds);
		if(ErrorBuf[errorcount]) LCD_WriteError(errorcount);
		if((++errorcount)>=ERROR_LEN_BUF) errorcount = 0;
	}
//	if((period & 0x0007)==0)
//	{
//		if(pwrcnt != PWR_Count)
//			PWR_On = 1;
//		else
//			PWR_On = 0;
//		pwrcnt = PWR_Count;
//	}

	 /* Sequence of Tasks:
	 Task1 -> Task2 -> Task1 -> Task3 -> Task1 -> Task2 -> Task1 -> Task3 -> ...
	
	 * Task 1: all Common Lines and all Segment Lines are in push-pull and are set to 0.
	 * Task 2: - Segment_lines[lcdcr] to be turned on are loaded with the value 1 otherwise 0.
						 - Common_line[lcdcr] is set to low. 
						 - Other Common lines are set to Vdd/2.
	 * Task 3: - Segment_lines[lcdcr] values are inverted.
						 - Common_line[lcdcr] is set to high.
						 - Other Common lines are set to Vdd/2.
	*/ 
 
		 
//	if(EnableTask1)					/*------------- Task 1 -------------*/
	{
	 /* All Segment lines = 0 ( all SegmentsLines_LQD_Port pins = 0 ) */
		 SegmentsLines_LQD_Port->BRR = (uint16_t)ALL_SEGMENT_LINES; 
		 SegmentsLines_HQD_Port->BRR = (uint16_t)ALL_SEGMENT_LINES;

		/* All Common lines = 0 */
		 CommonLines_Port->BRR = ALL_COMMON_LINES;				
		/* Configure all Common lines on CommonLines port pins as Out_PP */    
		CommonLines_Port->CRH &= 0x0000FFFF;
		CommonLines_Port->CRH |= ALL_COMMON_LINES_PP;
		
		for(uint32_t delay=0;delay<40;delay++);
			
		if(EnableTask2_3) /* If Task 2 has been executed previously (before Task 1) then prepare */
		{                         /* the RTC interrupt to execute Task 3 in the next interrupt */
		 /* Next interrupt will execute Task 3 and avoid it to execute Task 2 */
		 EnableTask2_3 = false;
		}
		else  /* If Task 3 has been executed previously (before Task 1) then prepare */
		{     /* the RTC interrupt to execute Task 2 in the next interrupt */
		 /* Next interrupt will execute Task 2 and avoid it to execute Task 3 */
		 EnableTask2_3 = true; 
		}
		/* Avoid next interrupt to execute Task 1, so the interrupt will execute Task 2 or Task 3 */ 
		EnableTask1 = false;
	}
//	else
	{ 
		if(EnableTask2_3)  /*------------- Task 2 --------------*/
		{
		 /* Segment_lines[lcdcr] to be turned on are loaded with the value 1 otherwise 0 */
			SegmentsLines_LQD_Port->BSRR = SegmentsValues_Lower_Quarter_Digits[lcdcr];  
			SegmentsLines_HQD_Port->BSRR = SegmentsValues_Higher_Quarter_Digits[lcdcr];
				 
			CommonLines_Port->BSRR = 0x0000F000;
			/* Common_line[lcdcr] is set to low */
			CommonLines_Port->BRR = CommonLine[lcdcr];
			/* Other Common lines set to Vdd/2 */
			CommonLines_Port->CRH &= CommonLine_VDD_2[lcdcr];
			/* Set Common_line[lcdcr] out push pull */
			CommonLines_Port->CRH |= CommonLine_OUT_PP[lcdcr];
				 
			/* Next interrupt will execute Task 1 */
			EnableTask1 = true;
		}
		else                 /*------------- Task 3 -------------*/
		{
			 /* Segment_lines[lcdcr] value		s are inverted */ 
			 SegmentsLines_LQD_Port->BSRR = 
									((uint32_t)(~SegmentsValues_Lower_Quarter_Digits[lcdcr])&0x000000FF);  
			 SegmentsLines_HQD_Port->BSRR = 
									((uint32_t)(~SegmentsValues_Higher_Quarter_Digits[lcdcr])&0x000000FF);

			/* Common_line[lcdcr] is set to high */
			CommonLines_Port->BRR = 0x0000F000;
			CommonLines_Port->BSRR = CommonLine[lcdcr];
			/* Other Common lines set to Vdd/2 */
			CommonLines_Port->CRH &= CommonLine_VDD_2[lcdcr];
			/* Other Common lines out push pull */
			 CommonLines_Port->CRH |= CommonLine_OUT_PP[lcdcr];

			/* Next interrupt will execute Task 1 */
			EnableTask1 = true;  
				 
			lcdcr++;
			if(lcdcr>3)
			{
			 lcdcr = 0;
			}
		}
	}
}

void LCD_Off(void)
{
	 /* All Segment lines = 0 ( all SegmentsLines_LQD_Port pins = 0 ) */
		 SegmentsLines_LQD_Port->BRR = (uint16_t)ALL_SEGMENT_LINES; 
		 SegmentsLines_HQD_Port->BRR = (uint16_t)ALL_SEGMENT_LINES;

		/* All Common lines = 0 */
		 CommonLines_Port->BRR = ALL_COMMON_LINES;				
		/* Configure all Common lines on CommonLines port pins as Out_PP */    
		CommonLines_Port->CRH &= 0x0000FFFF;
		CommonLines_Port->CRH |= ALL_COMMON_LINES_PP;
}


/**
* @brief  Включение всех индикаторов программ
* @param none
* @retval : None.
*/
void LCD_IndicatorOn(void)
{
	for(uint8_t i = 0; i<PROGRAM_NUM; i++)
		LCD_WriteIndicator(hndProgram[i].lcdind, IND_On);
}

/**
* @brief  Включение всех индикаторов программ
* @param none
* @retval : None.
*/
void LCD_IndicatorOff(void)
{
	for(uint8_t i = 0; i<PROGRAM_NUM; i++)
		LCD_WriteIndicator(hndProgram[i].lcdind, IND_Off);
}

/**
* @brief  Включение мигания индикатора
* @param none
* @retval : None.
*/
static tLCD_IND IndicatorBlink;
static void _TaskIndicatorBlink(void)
{
	static bool state = true;
	if(!state)
	{
		state = true;
		LCD_WriteIndicator(IndicatorBlink, IND_On);
	}
	else
	{
		state = false;
		LCD_WriteIndicator(IndicatorBlink, IND_Off);
	}
}

void LCD_IndicatorBlink(tLCD_IND ind)
{
	IndicatorBlink = ind;
	TaskIndicatorBlink = _TaskIndicatorBlink;
}


/*************************************************************/
