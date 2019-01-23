#include "rtc.h"
#include "lcd.h"
#include "led.h"
#include "sound.h"
#include "cmsis_os.h"
#include "programms.h"
#include "keyboard.h"
#include "stm32f1xx.h"   // Device header
#include <stdbool.h>
#include "term.h"

tPrgErr prgYogurtVoid(thndProgram *hnd);
tPrgErr prgVipechkaVoid(thndProgram *hnd);

#define HeaterUpOn()			HAL_GPIO_WritePin(GPIO_RELUP_GPIO_Port,GPIO_RELUP_Pin,GPIO_PIN_SET)
#define HeaterUpOff() 		HAL_GPIO_WritePin(GPIO_RELUP_GPIO_Port,GPIO_RELUP_Pin,GPIO_PIN_RESET)

#define HeaterDownOn() 		HAL_GPIO_WritePin(GPIO_RELDOWN_GPIO_Port,GPIO_RELDOWN_Pin,GPIO_PIN_SET)
#define HeaterDownOff() 	HAL_GPIO_WritePin(GPIO_RELDOWN_GPIO_Port,GPIO_RELDOWN_Pin,GPIO_PIN_RESET)

#define HeaterMidOn() 		HAL_GPIO_WritePin(GPIO_RELMID_GPIO_Port,GPIO_RELMID_Pin,GPIO_PIN_SET)
#define HeaterMidOff()		HAL_GPIO_WritePin(GPIO_RELMID_GPIO_Port,GPIO_RELMID_Pin,GPIO_PIN_RESET)

thndProgram hndProgram[PROGRAM_NUM] = { \
	{
		prgVipechkaVoid,				// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_RIS,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		105,											// minTemp
		105,											// maxTemp
		105,											// defaultTemp
		105,											// setTemp
		45,								// minTime
		45,								// maxTime
		45,								// defaultTime
		45								// setTime
	},
	{
		prgVipechkaVoid,				// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_GRECHKA,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		110,											// minTemp
		110,											// maxTemp
		110,											// defaultTemp
		110,											// setTemp
		50,								// minTime
		50,								// maxTime
		50,								// defaultTime
		50									// setTime
	},
	/*--- Молочная каша ---*/
	{
		prgYogurtVoid,					// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_MILK,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		98,											// minTemp
		98,											// maxTemp
		98,											// defaultTemp
		98,											// setTemp
		30,								// minTime
		120,								// maxTime
		50,								// defaultTime
		50									// setTime
	},
	/*--- Пар ---*/
	{
		prgYogurtVoid,					// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_PAR,					// lcdind
		LCD_IND_OVOCH,						// lcdind_second
		100,											// minTemp
		100,											// maxTemp
		100,											// defaultTemp
		100,											// setTemp
		5,								// minTime
		42,								// maxTime
		20,								// defaultTime
		20									// setTime
	},
	/*--- Томление ---*/
	{
		prgYogurtVoid,					// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_TOMLENIE,			// lcdind
		LCD_IND_OFF,						// lcdind_second
		45,										// minTemp
		45,										// maxTemp
		45,										// defaultTemp
		45,										// setTemp
		300,								// minTime
		600,								// maxTime
		300,								// defaultTime
		300									// setTime
	},
	/*--- Жарка ---*/
	{
		prgVipechkaVoid,				// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_JARKA,					// lcdind
		LCD_IND_OVOCH,						// lcdind_second
		180,											// minTemp
		180,											// maxTemp
		180,											// defaultTemp
		180,											// setTemp
		5,								// minTime
		59,								// maxTime
		42,								// defaultTime
		42									// setTime
	},
	/*--- Разогрев  ---*/
	{
		prgYogurtVoid,					// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_RAZOGREV,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		95,											// minTemp
		95,											// maxTemp
		95,											// defaultTemp
		95,											// setTemp
		8,								// minTime
		25,								// maxTime
		25,								// defaultTime
		25									// setTime
	},
	/*--- Выпечка ---*/
	{
		prgVipechkaVoid,				// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_VIPECHKA,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		130,											// minTemp
		130,											// maxTemp
		130,											// defaultTemp
		130,											// setTemp
		20,								// minTime
		120,								// maxTime
		60,								// defaultTime
		60									// setTime
	},
	/*--- Ручной  ---*/
	{
		prgVipechkaVoid,				// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_RUCHNOY,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		40,											// minTemp
		180,											// maxTemp
		100,											// defaultTemp
		100,											// setTemp
		10,								// minTime
		600,								// maxTime
		45,								// defaultTime
		45									// setTime
	},
	/*--- Тушение ---*/
	{
		prgVipechkaVoid,				// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_TUSHENIE,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		100,											// minTemp
		100,											// maxTemp
		100,											// defaultTemp
		100,											// setTemp
		20,								// minTime
		240,								// maxTime
		60,								// defaultTime
		60									// setTime
	},
	/*----- Йогурт -----*/
	{
		prgYogurtVoid,					// handlerStart
		NULL,										// handlerTime
		NULL,										// handlerMode
		NULL,										// handlerDelay
		LCD_IND_YOGURT,					// lcdind
		LCD_IND_OFF,						// lcdind_second
		40,											// minTemp
		40,											// maxTemp
		40,											// defaultTemp
		40,											// setTemp
		360,								// minTime
		960,								// maxTime
		720,								// defaultTime
		720									// setTime
	}
};

void BlinkHour(void);
void BlinkMinute(void);
void OutTime(void);
void OutTimeToEnd(void);
void OutTimeToEndPodogrev(void);
void SetTime(RTC_TimeTypeDef *time);
tPrgErr CookingControl(uint32_t timeCooking, uint16_t tempCooking, 
										uint16_t deltaDownMin, uint16_t deltaDownMax,
										uint16_t deltaMidMin, uint16_t deltaMidMax,
										uint16_t deltaUpMin, uint16_t deltaUpMax);
void LEDProgresBar(uint32_t maxTime, uint32_t time);

extern void (*TaskBlink)(void);
extern void (*TaskIndicatorBlink)(void);

RTC_TimeTypeDef time;
uint8_t	temp;	//показания температуры

static uint32_t TimeToEnd;
static uint32_t TimeCooking;

#define DELAY   50
#define TIMEOUT 8000/DELAY
#define TIMEOUT2 20000/DELAY

/********************************************************
*   Программа установки часов, мигает изменяемый пераметр
* времени
*********************************************************/
void prgTimeSet(void)
{
	uint16_t timeout=0;		// контроль за неактивностью нажатий в течении 8 секунд
	uint8_t getkey=0;						// сохранение нажатой кнопки
	Beep();
	HAL_RTC_GetTime(&hrtc, &time, FORMAT_BIN);
	LCD_WriteIndicator(LCD_IND_POINTS,IND_On);
	TaskBlink = BlinkHour;			// устанавливаем мигание значения часов

	uint16_t timlim = 80;		//
	while((timeout++)<TIMEOUT)
	{

		
		/*---- Обработка нажатой кнопки ---- */
		if(KEYPressed())
			{
			timeout = 0;
			getkey = GetKeys();
			switch(getkey)
			{
				case KEY_TIME:
					/*---- Приращение показаний времени ---- */
					if(TaskBlink == BlinkHour)
					{
						if(time.Hours == 23)
							time.Hours = 0;
						else
							time.Hours++;
					}
					else
					{
						if(time.Minutes == 59)
							time.Minutes = 0;
						else
							time.Minutes++;
					}
					break;
				case KEY_DELAY:
					/*---- Переключение на изменеие минут или часов ---- */
					{
						timlim = 80;
						if(TaskBlink == BlinkHour)
						{
							TaskBlink = BlinkMinute;
							LCD_WriteHour(time.Hours);
						}
						else
						{
							LCD_WriteMinutes(time.Minutes);
							TaskBlink = BlinkHour;
						}
					};
					break;
				case KEY_CANCEL:
					/*---- увеличиваем контроль неактивности для выхода из изменеия времени ---- */
					timeout = 200;
					timlim = 80;
					break;
			};
		};
		
		
		uint16_t timeout2=0;
		while(KEYPressed() && (timeout2 < timlim))
			/*---- Обработка отпускания кнопки или вовторного нажатия для изменеия показаний ---- */
		{
			if(getkey == KEY_TIME)
			{
				timeout2++;
				osDelay(50);
			}
		};
		if(KEYPressed())
			timlim = 8;
		else
			timlim = 80;
		osDelay(DELAY);
	};
	
	TaskBlink = NULL;
	time.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &time, FORMAT_BIN);
	Beep();
};

/********************************************************
*   Программа дежурного режима
* Все индикаторы выключены, работает опрос кнопок и 
* выводится текущее время
*********************************************************/
tPrgErr prgStandbyMode(void)
{
	Beep();
	
	for(uint8_t i = 0; i<PROGRAM_NUM; i++)
		LCD_WriteIndicator(hndProgram[i].lcdind, IND_Off);

	LCD_WriteIndicator(LCD_IND_TERM, IND_Off);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_Off);
	LCD_IndicatorOn();
	
	LCD_WriteChar(CHAR_SPACE, 4);
	LCD_WriteChar(CHAR_SPACE, 5);
	LCD_WriteChar(CHAR_SPACE, 6);

	for(uint8_t i = 0; i<9 ; i++)  // выключаем все светодиоды
		LED_Control(1<<i, LED_Off);
	
	TaskBlink = OutTime;						// устанавливаем задачу для вывода текущего времени
	TaskIndicatorBlink=NULL;				// задачу мигания отключаем
	
	return PRG_END_OK;
};

/********************************************************
*   Программа подогрева
* Все индикатор последнего режима выключен, работает опрос кнопки отмена и 
* выводится текущее время
*********************************************************/
#define PODOGREV_TEMP	75
#define PODOGREV_TIME 360

tPrgErr prgPodogrev(void)
{
	Beep();

	LCD_WriteIndicator(LCD_IND_TERM, IND_Off);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_Off);
	LCD_IndicatorOn();
	
	LCD_WriteChar(CHAR_SPACE, 4);
	LCD_WriteChar(CHAR_SPACE, 5);
	LCD_WriteChar(CHAR_SPACE, 6);

	for(uint8_t i = 0; i<9 ; i++)
		LED_Control(1<<i, LED_Off);
	
	LED_Control(LED_Cancel, LED_On);
	TaskBlink = OutTimeToEndPodogrev;
	TaskIndicatorBlink=NULL;
	
	return CookingControl(PODOGREV_TIME, PODOGREV_TEMP, PODOGREV_TEMP-5, PODOGREV_TEMP-3, PODOGREV_TEMP-2,
												PODOGREV_TEMP, PODOGREV_TEMP-3, PODOGREV_TEMP-1);

};

/********************************************************
*   Программа "йогурт"
*
*********************************************************/
tPrgErr prgYogurtVoid(thndProgram *hnd)
{

	LCD_WriteIndicator(LCD_IND_POINTS, IND_On);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_On);

	TaskBlink = OutTimeToEnd;

	return CookingControl(hnd->setTime, hnd->setTemp, hnd->setTemp-5, hnd->setTemp-3, 
								hnd->setTemp-2, hnd->setTemp, hnd->setTemp-3, hnd->setTemp-1);
};


/********************************************************
*   Программа "выпечка"
*
*********************************************************/
tPrgErr prgVipechkaVoid(thndProgram *hnd)
{
	LCD_WriteIndicator(LCD_IND_POINTS, IND_On);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_On);

	TaskBlink = OutTimeToEnd;

	return CookingControl(hnd->setTime, hnd->setTemp, hnd->setTemp-8, hnd->setTemp-6, 
								hnd->setTemp-5, hnd->setTemp-2, hnd->setTemp-1, hnd->setTemp+1);
};

/********************************************************
*   Мигание показаний часов при изменении 
*
*********************************************************/
void BlinkTemp(void)
{
	static bool i=true;
	if(i)
	{
		LCD_WriteTemp(temp);
		i=false;
	}
	else
	{
		LCD_WriteChar(11,4);
		LCD_WriteChar(11,5);
		LCD_WriteChar(11,6);
		i=true;
	}
	osDelay(200);
};

/********************************************************
*   Мигание показаний часов при изменении 
*
*********************************************************/
void BlinkHour(void)
{
	static bool i=true;
	if(i)
	{
			LCD_WriteHour(time.Hours);
		i=false;
	}
	else
	{
		LCD_WriteChar(11,0);
		LCD_WriteChar(11,1);
		i=true;
	}
	osDelay(200);
};

/********************************************************
*   Мигание показаний минут при изменении
*
*********************************************************/
void BlinkMinute(void)
{
	static bool i=true;
	if(i)
	{
			LCD_WriteMinutes(time.Minutes);
		i=false;
	}
	else
	{
		LCD_WriteChar(11,2);
		LCD_WriteChar(11,3);
		i=true;
	}
	osDelay(200);
};


void SetTime(RTC_TimeTypeDef *time)
{

};
/********************************************************
*   Установка времениприготовления для выбранной
* программы
*********************************************************/
void SetTimeCooking(thndProgram *hnd)
{
	uint16_t timeout=0;		// контроль за неактивностью нажатий в течении 8 секунд
	uint16_t minutemax, minutemin, minute; // значения для перевода времени  в минуты
	uint8_t getkey=0;						// сохранение нажатой кнопки
	Beep();
	LCD_WriteIndicator(LCD_IND_POINTS,IND_On);
	TaskBlink = BlinkHour;			// устанавливаем мигание значения часов
	
	minute = hnd->setTime;
	time.Hours = minute/60;
	time.Minutes = minute%60;
	minutemax = hnd->maxTime;
	minutemin = hnd->minTime;

	uint16_t timlim = 80;		//
	while((timeout++)<TIMEOUT2)
	{

		
		/*---- Обработка нажатой кнопки ---- */
		if(KEYPressed())
			{
			timeout = 0;
			getkey = GetKeys();
			switch(getkey)
			{
				case KEY_TIME:
					/*---- Приращение показаний времени ---- */
					if(TaskBlink == BlinkHour)
					{
						if(minutemax>59)
							if((minutemax/60) > (minutemin/60))
							{
								minute += 60;
								if((minute/60) > (minutemax/60))
									minute = minutemin/60 + minute%60;
								time.Hours = minute / 60;
								time.Minutes = minute % 60;
							}
					}
					else
					{
						if(minutemax>59){
							if(time.Minutes >=  59)
								time.Minutes = 0;
							else
								time.Minutes++;
							minute = time.Hours*60 + time.Minutes;
						}
						else
						{
							if(time.Minutes >=  minutemax)
								time.Minutes = minutemin;
							else
								time.Minutes++;
							minute = time.Minutes;
						};
					}
					break;
				case KEY_TIMECOOKING:
					/*---- Переключение на изменеие минут или часов ---- */
					timlim = 80;
					if(minute>minutemax) minute=minutemax;
					if(minute<minutemin) minute=minutemin;

					time.Hours = minute/60;
					time.Minutes = minute%60;
				
					{
						if(TaskBlink == BlinkHour)
						{
							TaskBlink = BlinkMinute;
							LCD_WriteHour(time.Hours);
						}
						else
						{
							LCD_WriteMinutes(time.Minutes);
							TaskBlink = BlinkHour;
						}
					};
					break;
				case KEY_START:
					/*---- сохраняем измененнное время и выходим ---- */
					if(minute>minutemax) minute=minutemax;
					if(minute<minutemin) minute=minutemin;

					hnd->setTime = minute;
					timeout = TIMEOUT2 + 20;
					break;
				case KEY_CANCEL:
					/*---- выходим без изменения ---- */
					timeout = TIMEOUT2 + 20;
					break;
			};
		};
		
		
		uint16_t timeout2=0;
		while(KEYPressed() && (timeout2 < timlim))
			/*---- Обработка отпускания кнопки или вовторного нажатия для изменеия показаний ---- */
		{
			if(getkey == KEY_TIME)
			{
				timeout2++;
				osDelay(50);
			}
		};
		if(KEYPressed())
			timlim = 8;
		else
			timlim = 80;
		osDelay(DELAY);
	};
	
	TaskBlink = NULL;
	time.Seconds = 0;
	Beep();
}




/********************************************************
*	SetTempCooking
*   Установка температуры приготовления для выбранной
* программы
*********************************************************/
void SetTempCooking(thndProgram *hnd)
{
	uint16_t timeout=0;		// контроль за неактивностью нажатий в течении 8 секунд
	uint8_t getkey=0;						// сохранение нажатой кнопки
	Beep();
	temp = hnd->setTemp;
	LCD_WriteIndicator(LCD_IND_TERM,IND_On);
	TaskBlink = BlinkTemp;			// устанавливаем мигание значения температуры
	
	uint16_t timlim = 80;		//
	while((timeout++)<TIMEOUT2)
	{

		
		/*---- Обработка нажатой кнопки ---- */
		if(KEYPressed())
			{
			timeout = 0;
			getkey = GetKeys();
			switch(getkey)
			{
				case KEY_TEMP:
					/*---- Приращение показаний температуры ---- */
					if(temp < hnd->maxTemp)
						temp += 10;
					else
						temp = hnd->minTemp;
					break;
				case KEY_START:
					/*---- сохраняем измененнное время и выходим ---- */
					hnd->setTemp = temp;
					timeout = TIMEOUT2 + 20;
					break;
				case KEY_CANCEL:
					/*---- выходим без изменения ---- */
					timeout = TIMEOUT2 + 20;
					break;
			};
			Beep();

		};
		
		
		uint16_t timeout2=0;
		while(KEYPressed() && (timeout2 < timlim))
			/*---- Обработка отпускания кнопки или вовторного нажатия для изменеия показаний ---- */
		{
			if(getkey == KEY_TIME)
			{
				timeout2++;
				osDelay(50);
			}
		};
		if(KEYPressed())
			timlim = 8;
		else
			timlim = 80;
		osDelay(DELAY);
	};
	
	TaskBlink = NULL;
	time.Seconds = 0;
}
/************** End function  SetTempCooking ************/


/********************************************************
*   Функция вывода показаний часов
*
*********************************************************/
void OutTime(void)
{
//	static uint8_t i=0;
	
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, FORMAT_BIN);
	LCD_WriteTime(time.Hours, time.Minutes);
//	if((i%4) == 0)
		LCD_WriteIndicator(LCD_IND_POINTS, IND_Toggle);
	osDelay(500);
};

/********************************************************
*   Функция вывода времени до окончания программы
*
*********************************************************/
void OutTimeToEnd(void)
{
	LCD_WriteTime((TimeToEnd / 60), ((uint8_t)(TimeToEnd % 60)));
	LCD_WriteIndicator(LCD_IND_POINTS, IND_Toggle);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_Toggle);
	
	LEDProgresBar(TimeCooking, TimeToEnd);
	osDelay(500);
};

/********************************************************
*   Функция вывода времени до окончания программы
*		подогрев
*********************************************************/
void OutTimeToEndPodogrev(void)
{
	LCD_WriteHour(TimeToEnd / 60 );
	LCD_WriteChar(CHAR_h, 2);
	LCD_WriteChar(CHAR_SPACE, 3);
	LCD_WriteIndicator(LCD_IND_POINTS, IND_Off);
	LCD_WriteIndicator(LCD_IND_CLOCK, IND_Toggle);
	osDelay(500);
};
/********************************************************
*    CookingControl
*		запуск нагрева готовки.
*  deltadown - дельта нагрева нижнего тена
*  deltamid - дельта нагрева среднего тена
*  deltaup - дельта нагрева вернехо тена
*
*********************************************************/
tPrgErr CookingControl(uint32_t timeCooking, uint16_t tempCooking, 
										uint16_t deltaDownMin, uint16_t deltaDownMax,
										uint16_t deltaMidMin, uint16_t deltaMidMax,
										uint16_t deltaUpMin, uint16_t deltaUpMax)
{
	uint8_t getkey=0;
	
	TimeCooking = timeCooking;
	volatile uint32_t timecurrent = RTC_ReadTimeCounter(&hrtc)/60;

	while(getkey != KEY_CANCEL)
	{
		TimeToEnd = timeCooking - (RTC_ReadTimeCounter(&hrtc)/60-timecurrent);
		LCD_WriteIndicator(LCD_IND_TERM, IND_On);
		LCD_WriteTemp(TERM_GetTemp(TermDown));
		getkey = GetKeys();

		while((TERM_GetTemp(TermDown) < (tempCooking+1)) && (getkey != KEY_CANCEL))
		{
			if(TERM_GetTemp(TermDown) < deltaDownMin)
				HeaterDownOn();
			else if(TERM_GetTemp(TermDown) >= deltaDownMax)
				HeaterDownOff();

			if(TERM_GetTemp(TermDown) < deltaMidMin)
				HeaterMidOn();
			else if(TERM_GetTemp(TermDown) >= deltaMidMax)
				HeaterMidOff();

			if(TERM_GetTemp(TermDown) < deltaUpMin)
				HeaterUpOn();
			else if(TERM_GetTemp(TermDown) >= deltaUpMax)
				HeaterUpOff();

			getkey = GetKeys();
			osDelay(200);
			TimeToEnd = timeCooking - (RTC_ReadTimeCounter(&hrtc)/60-timecurrent);
			if(TimeToEnd == 0) getkey = KEY_CANCEL;
			LCD_WriteIndicator(LCD_IND_TERM, IND_Toggle);
			LCD_WriteTemp(TERM_GetTemp(TermDown));
		}

		if(TimeToEnd == 0) getkey = KEY_CANCEL;
		osDelay(200);
		HeaterMidOff();
		HeaterUpOff();
		HeaterDownOff();
	}

	LED_Control(LED_1|LED_2|LED_3|LED_4|LED_5, LED_Off);

	if(getkey == KEY_CANCEL)
		return PRG_END_CANCEL;
	else
		return PRG_END_OK;
};
/************** End function  CookingControl ************/

/********************************************************
*   LEDProgresBar
*		вывод прогресса на светодиоды - этапы приготовления
*********************************************************/

void LEDProgresBar(uint32_t maxTime, uint32_t time)
{
//	uint32_t l = time*5/maxTime;
	
	switch((uint32_t)(time*5/maxTime))
  {
  	case 5:
			LED_Control(LED_1, LED_On);
  		break;
  	case 4:
			LED_Control(LED_1|LED_2, LED_On);
  		break;
  	case 3:
			LED_Control(LED_1|LED_2|LED_3, LED_On);
  		break;
  	case 2:
			LED_Control(LED_1|LED_2|LED_3|LED_4, LED_On);
  		break;
  	case 1:
			LED_Control(LED_1|LED_2|LED_3|LED_4|LED_5, LED_On);
  		break;
  	default:
  		break;
  }
}
