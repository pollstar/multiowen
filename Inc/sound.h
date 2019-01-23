/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sound_H
#define __sound_H

#include "cmsis_os.h"

#ifdef __cplusplus
 extern "C" {
#endif

void Beep(void);
void BeepTime(uint16_t delay);
void BeepSetTone(uint16_t tone);
	 
#ifdef __cplusplus
}
#endif
#endif /*__ sound_H */


/*****************************END OF FILE****/

