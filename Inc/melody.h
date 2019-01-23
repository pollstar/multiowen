#ifndef __melody_H
#define __melody_H

#include "cmsis_os.h"
#include "string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef 	enum eMelody {mldBumer = 0, mldStarWars, mldUkraine} teMelody;

typedef 	char* tMelody;
typedef enum {playERR = 0, playOK} terrPlay;

const tMelody mldsBumer = "8e2 4g2 4- 8- 8g2 4e2 4- 8- 16.a2 16.g2 16.a2 16.g2 16.a2 16.g2 16.a2 16.g2 16.a2 4b2";
const tMelody mldsStarWars = "8#c1 8#c1 16#c1 2#f1 2#c2 8b1 16#a1 8#g1 2#f2 4#c2 8b1 \
														16#a1 8#g1 2#f2 4#c2 8b1 16#a1 8b1 2#g1 8#c1 8#c1 16#c1 \
														2#f1 2#c2 8b1 16#a1 8#g1 2#f2 4#c2 8b1 16#a1 8#g1 2#f2 \
														4#c2 8b1 16#a1 8b1 2#g1 4#c1 16#c1 2#d1 8#c2 8b1 8#a1 \
														8#g1 8#f1 16#f1 8#g1 16#a1 4#g1";
const tMelody mldsUkraine = "";
	
terrPlay PlayMelody(tMelody melody);
	 
#ifdef __cplusplus
}
#endif
#endif /*__ melody_H */


/*****************************END OF FILE****/

