#include "stm32f1xx.h"   // Device header
#include "gpio.h"
#include "lcd.h"
#include "term.h"
#include "led.h"
#include "sound.h"
#include "tim.h"
#include "rtc.h"
#include "power.h"
#include "programms.h"
#include <stdbool.h>
#include "error.h"

uint8_t PWR_On = 0;
 
PWR_PVDTypeDef hpvd;
//void MX_PVD_Init(void);

uint32_t PWR_Count=0;



/*******************************************************
* Минимальная инициализация системы при подаче питания
*
********************************************************/


/******************************************************
* Функция переводящая в полноценные рабочий режим при
* подключении сетевого питания
*******************************************************/
void PowerFull(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

	HAL_RCC_DeInit();

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSIState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

	MX_GPIO_Init();
//	MX_RTC_Init();
	LCD_GPIO_Init();
	LED_GPIO_Init();
	/* Настройка и запуск таймера вывода индикации LCD */
//	MX_TIM17_DeInit();
	MX_TIM17_Init(LCD_TIM_CLOCK_MAX);

  /* Initialize all configured peripherals */
	TERM_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin PBPin PBPin PBPin 
                           PBPin */
  GPIO_InitStruct.Pin = GPIO_Led_Pin|GPIO_RELUP_Pin|GPIO_RELMID_Pin|GPIO_RELDOWN_Pin|GPIO_ONCONTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_Buz2_Pin|GPIO_Buz1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIO_Buz1_GPIO_Port, &GPIO_InitStruct);

/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_Buz2_Pin|GPIO_Buz1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_X1_Pin|GPIO_X2_Pin|GPIO_X3_Pin|GPIO_Y1_Pin 
                          |GPIO_Y2_Pin|GPIO_Y3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_Led_Pin|GPIO_RELUP_Pin|GPIO_RELMID_Pin|GPIO_RELDOWN_Pin 
                          |GPIO_ONCONTRL_Pin, GPIO_PIN_RESET);
};


/******************************************************
* Функция переводящая в дежурный режим при
* отключении сетевого питания и питании от бабатерии
* при этом работаюттолько часы и контроль за подачей 
* сетевого напряжения, 
* вход в спящий режим.
*******************************************************/
void PowerMin(void)
{
	HAL_RCC_DeInit();

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  ErrorBuf[ERROR_RCC_OSC] =	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV64;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  ErrorBuf[ERROR_RCC_CLOCK] = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  ErrorBuf[ERROR_RCC_PEREFCLK] = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);


	MX_GPIO_Init();
	MX_RTC_Init();
	LCD_GPIO_Init();
//	MX_PVD_Init();
	/* Настройка и запуск таймера вывода индикации LCD */
//	MX_TIM17_DeInit();
	MX_TIM17_Init(LCD_TIM_CLOCK_MIN);

	
	
};

////void MX_PVD_Init(void)
////{
////	hpvd.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
////	hpvd.PVDLevel = PWR_PVDLEVEL_7;
////	HAL_PWR_ConfigPVD(&hpvd);
////	
////	HAL_PWR_EnablePVD();
////};

////void HAL_PWR_PVDCallback(void)
////{
////	if(PWR->CSR & PWR_CSR_PVDO)
////		PWR_On |= 2;
////	else
////		PWR_On = 0x0;

////}
/********************************************************
*   Полное выключение всех программ, опроса кнопок и вывод
*	светодиодов, используется для остановки при жесткой ошибке
*********************************************************/

void PowerHardOff(void)
{
	Beep();
	for(uint8_t i = 0; i<PROGRAM_NUM; i++)
	LCD_WriteIndicator(hndProgram[i].lcdind, IND_Off);

	LCD_WriteIndicator(LCD_IND_TERM, IND_Off);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_Off);
	LCD_WriteChar(CHAR_SPACE, 4);
	LCD_WriteChar(CHAR_SPACE, 5);
	LCD_WriteChar(CHAR_SPACE, 6);

	for(uint8_t i = 0; i<9 ; i++)
		LED_Control(1<<i, LED_Off);

};
/************** End function PowerHardOff ****************/

