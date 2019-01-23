/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "rtc.h"
#include "adc.h"
#include "led.h"
#include "term.h"
#include "lcd.h"
#include "keyboard.h"
#include "power.h"
#include "programms.h"
#include "stm32f1xx_it.h"
#include "sound.h"
#include "error.h"

//extern uint8_t PWR_On;

/* Variables -----------------------------------------------------------------*/
osThreadId mainTaskHandle;
osThreadId timeTaskHandle;
osThreadId pwrTaskHandle;
//extern thndProgram hndProgram;

void (*TaskBlink)(void);
void (*TaskIndicatorBlink)(void);

volatile static bool flgTimeOut = true;

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartTimeTask(void const * argument);
void StartPWRTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */


/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* Create the thread(s) */
  /* definition and creation of mainTask */
  osThreadDef(mainTask, StartDefaultTask, osPriorityNormal, 0, 128);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

	osThreadDef(timeTask, StartTimeTask, osPriorityAboveNormal, 0, 128);
	timeTaskHandle = osThreadCreate(osThread(timeTask), NULL);

	osThreadDef(pwrTask, StartPWRTask, osPriorityAboveNormal, 0, 128);
	pwrTaskHandle = osThreadCreate(osThread(pwrTask), NULL);

	TaskBlink = 0;
	TaskIndicatorBlink = NULL;
}

void termout(void)
{
	LCD_WriteTemp(TERM_GetTemp(TermDown));
	uint16_t term = termADCValue[0];
	LCD_WriteChar(term%10,3);		// значение АЦП терморезистора
	term /= 10;
	LCD_WriteChar(term%10,2);		// значение АЦП терморезистора
	term /= 10;
	LCD_WriteChar(term%10,1);		// значение АЦП терморезистора
	term /= 10;
	LCD_WriteChar(term,0);		// значение АЦП терморезистора

	osDelay(500);
};

/* StartDefaultTask function 
* Основная задача выбора и запуска программ, задания основных параметров
*/

void StartDefaultTask(void const * argument)
{

	HAL_GPIO_WritePin(GPIO_Buz1_GPIO_Port, GPIO_Buz1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO_Buz2_GPIO_Port, GPIO_Buz2_Pin, GPIO_PIN_RESET);
	
	static uint8_t programm_current = PROGRAM_NUM+10; // увеличиваем текущий указатель программы на 10 от максимальной программы
																										//что указывает что программа не выбрана

	/*--- Запускаем дежурный режим --*/
	prgStandbyMode();  

  while(1)
  {
			
		/*--- Здесь  устроить проверку на бездействие и переход в дежурный режим */
		uint32_t timenotpress = 0; // счетчик времени бездействия

		// Ждем пока нет нажатия кнопки
		while(!KEYPressed()) 
		{
			timenotpress++; // увеличиваем счетчик времени бездействия
			// если счетчик бездействия вышел за предел и была выбранна программа 
			if((timenotpress>750) && (programm_current < PROGRAM_NUM))
			{
				timenotpress=0; //обнуляем счетчик
				programm_current = PROGRAM_NUM+10; //флаг что программа не выбранна
				prgStandbyMode(); //переход в дежурный режим
				flgTimeOut = true; //флаг вывода текущего времени
			};
			osDelay(30);
		};

		uint32_t timepress = 0; //счетчик длительности нажатия кнопки
		uint8_t getkey = GetKeys(); //чтение кнопки
		while((KEYPressed()) && (timepress <= 40)) //пока кнопка нажата увеличиваем счетчик
			{																					// чтобы определить долгое нажатие
				timepress++;
				osDelay(30);
			};
	//		Beep();
			switch(getkey){
				case KEY_MENU:
					Beep();
					TaskBlink = NULL;
					LCD_WriteIndicator(hndProgram[programm_current++].lcdind, IND_On);
					if(programm_current >= PROGRAM_NUM)
						programm_current = 0;
					flgTimeOut = false;
					LCD_IndicatorBlink(hndProgram[programm_current].lcdind);
					LCD_WriteIndicator(LCD_IND_TERM, IND_On);
					LCD_WriteIndicator(LCD_IND_CLOCK, IND_On);
					LCD_WriteIndicator(LCD_IND_POINTS, IND_On);
					LCD_WriteTemp(hndProgram[programm_current].setTemp);
					LCD_WriteTime(hndProgram[programm_current].setTime/60, \
												hndProgram[programm_current].setTime%60);
					break;
				case KEY_START:
					if(programm_current < PROGRAM_NUM)
					{
						LED_Control(LED_Start, LED_On);
						if(hndProgram[programm_current].handlerStart != NULL)
						{
							Beep();
							LCD_IndicatorOff();
							TaskIndicatorBlink = NULL;
							LCD_WriteIndicator(hndProgram[programm_current].lcdind, IND_On);

							tPrgErr err = hndProgram[programm_current].handlerStart(&hndProgram[programm_current]);
							for(uint8_t i=0; i<4; i++)
							{
								BeepTime(500);
								osDelay(300);
							}
							if(hndProgram[programm_current].lcdind != LCD_IND_YOGURT)
								if(err == PRG_END_OK)
									prgPodogrev();
						}
						else
						{
							BeepTime(100);
							osDelay(100);
							BeepTime(100);
							osDelay(100);
							BeepTime(100);
							osDelay(100);
						}
						LED_Control(LED_Start, LED_Off);
						programm_current = PROGRAM_NUM+10;
						prgStandbyMode();
					}
//				LCD_WriteChar(num++,0);
	//				if(num>11) num=0;
					break;
				case KEY_CANCEL:
					// если не выбранна ни одна программа, просто запускаем режим подогрева
					if(programm_current > PROGRAM_NUM+1)
						prgPodogrev();
					// переходим в дежурный режми
					programm_current = PROGRAM_NUM+10;
					prgStandbyMode();
					flgTimeOut = true;
					break;
				case KEY_DELAY:
					if(timepress>30)
					{
						flgTimeOut = false;
						prgTimeSet();
						flgTimeOut = true;
					};
					break;
				case KEY_SELECT:
					Beep();
					break;
				case KEY_TEMP:
					if(programm_current<PROGRAM_NUM)
					{
						if((hndProgram[programm_current].defaultTemp != hndProgram[programm_current].minTemp)||
								(hndProgram[programm_current].defaultTemp != hndProgram[programm_current].maxTemp))
							SetTempCooking(&hndProgram[programm_current]);
						LCD_WriteTemp(hndProgram[programm_current].setTemp);
					};
					break;
	//			case KEY_TIME:
	//				LED_Control(LED_5, LED_Toggle);
//					break;
				case KEY_TIMECOOKING:
					if(programm_current<PROGRAM_NUM)
					{
						if((hndProgram[programm_current].lcdind != LCD_IND_RIS)&&
								(hndProgram[programm_current].lcdind != LCD_IND_GRECHKA))
						SetTimeCooking(&hndProgram[programm_current]);
						LCD_WriteTime(hndProgram[programm_current].setTime/60, \
													hndProgram[programm_current].setTime%60);
					};
					break;
			}
			osDelay(10);
		}
//		vTaskEndScheduler();
//		HAL_NVIC_SystemReset();
}

/* StartTimeTask function */
void StartTimeTask(void const * argument)
{
	for(;;)
	{
		if(TaskBlink)
		{
			if(TaskIndicatorBlink)
			{
				(*TaskIndicatorBlink)();
			}
			(*TaskBlink)();
		}
		else
		{
			if(TaskIndicatorBlink)
			{
				(*TaskIndicatorBlink)();
			}
			osDelay(250);
		}
		
	};
}  /* END StartTimeTask */

/* StartPWRTask function */
void StartPWRTask(void const * argument)
{
	static uint32_t pr=0;
	for(;;)
	{
		if(PWR_Count == pr)
		{
			PWR_On = false;
			HAL_NVIC_SystemReset();
		}
		pr = PWR_Count;

		if (HAL_ADCEx_InjectedStart_IT(&hadc1) != HAL_OK)
		{
			/* Start Error */
			Error_Handler(ERROR_E4);
		}
		osDelay(500);
	};
}  /* END StartPWRTask */

/*****************************END OF FILE*******************************/


