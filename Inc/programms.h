/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __programms_H
#define __programms_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>
#include "rtc.h"
#include "lcd.h"

#define PROGRAM_NUM 11  // Количество программ

enum ePrgErr{PRG_END_OK = 0, PRG_END_CANCEL, PRG_END_ERROR};

typedef enum ePrgErr tPrgErr;
	 
typedef struct shndProgram thndProgram;

struct shndProgram{
	tPrgErr (*handlerStart)(thndProgram *hnd);		// обработчик программы запущенной на выполнение
	tPrgErr (*handlerTime)(thndProgram *hnd);		// обработчик установки времени приготовления
	tPrgErr (*handlerMode)(thndProgram *hnd);		// обработчик установки допонительных режимов
	tPrgErr (*handlerDelay)(thndProgram *hnd);		// обработчик отсрочки программы
	const tLCD_IND lcdind;
	const tLCD_IND lcdind_second;
	const uint8_t minTemp;
	const uint8_t maxTemp;
	const uint8_t defaultTemp;
	uint8_t	setTemp;
	uint16_t minTime;
	uint16_t maxTime;
	uint16_t defaultTime;
	uint16_t	setTime;
};

extern thndProgram hndProgram[];
	 
void prgTimeSet(void);
tPrgErr prgStandbyMode(void);
tPrgErr prgPodogrev(void);
void SetTimeCooking(thndProgram *hnd);
void SetTempCooking(thndProgram *hnd);

extern void (*TaskBlink)(void);
extern void (*TaskIndicatorBlink)(void);
extern void (*TaskOutTime)(void);
extern void (*TaskOutTimer)(void);
extern void (*TaskOutTemp)(void);
	 
#ifdef __cplusplus
}
#endif
#endif /*__ programms_H */


/*****************************END OF FILE****/


