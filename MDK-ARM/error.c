#include "error.h"
#include <stdint.h>
#include "lcd.h"
#include "power.h"

uint8_t ErrorBuf[ERROR_LEN_BUF];
uint8_t errorFlag=0;			// есть ошибки в буфере
uint8_t errorCurrent=0;			// всего ошибок в буфере

/********************************************************
* Вывод сообщенияоб ошибке, при необходимости полный 
* приготовления
*********************************************************/
void Error_Handler(uint8_t err)
{
	LCD_WriteIndicator(LCD_IND_POINTS, IND_Off);
	if (err<0x80)
	{ //-- обработка не критичных ошибок
		LCD_WriteError(err);
	}
	else
	{
		LCD_WriteError(err-0x80);
	}
};
/************** End function ErrorHandler*****************/


