/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "cmsis_os.h"

#include "term.h"
#include "lcd.h"
#include "rtc.h"
#include "tim.h"
#include "led.h"
#include "sound.h"
#include "gpio.h"
#include "power.h"

/* Private variables ---------------------------------------------------------*/
uint32_t DebugDelay;  
//extern uint8_t PWR_On;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);


int main(void)
{

	/*-Минимальные настройки дя отображения часов и переход в спящий режим--*/
  /* Enable Debug in low power modes in the DBGMCU_CR register */

//  * (__IO uint32_t *) 0xE0042004 = 0x00000007;
  
  /* Debug delay */
//  for (DebugDelay=1000000; DebugDelay!=0; DebugDelay--);    
	
	
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	PWR_On = false;
	PWR_Count=0xFFFF;

	while(1)
	{
		PowerMin();
		/* Настройка и запуск таймера вывода индикации LCD */
		
		LCD_WriteIndicator(LCD_IND_CLOCK, IND_On);
		LCD_WriteIndicator(LCD_IND_MIASO, IND_Off);
		
		while(!PWR_On){
			__wfi();
		};

		PowerFull();

		LCD_WriteIndicator(LCD_IND_CLOCK, IND_Off);

	  /* Call init function for freertos objects (in freertos.c) */
		MX_FREERTOS_Init();

	  /* Start scheduler */
		osKernelStart();

		while(PWR_On){
	//	while(1)	{
		};
		vTaskEndScheduler();
		LED_Light(LED_Off);	
		LCD_Off();

		/* DeInitialize all configured peripherals */
		MX_TIM7_DeInit();
		MX_TIM16_DeInit();
		TERM_DeInit();
		
	};
}


/*****************************END OF FILE****/
