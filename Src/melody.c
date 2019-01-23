#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "tim.h"
#include "sound.h"
#include "melody.h"
#include <stdio.h>

/* ------ Частоты нот первой октавы ----------*/
typedef struct NoteFreq{
	char*  			note;
	uint16_t 		freq;
}tNoteFreq;

#define NUMNOTES 14 //Количество нот в октаве, с паузой
tNoteFreq Notes[NUMNOTES] = {"C" , 261.626*20, "#C" , 277.183*20, "D" , 293.665*20, "#D" , 311.127*20, \
														"E", 329.628*20, "#E" , 349.228*20, "F" , 349.228*20, "#F", 369.994*20, "G", 391.995*20, \
														"#G" , 415.305*20, "A" , 440.000*20, "#A" , 466.164*20, "B" , 493.883*20, "#B" , 523.251*20};

typedef enum {OCTAVESUBCONTR = 0, OCTAVECONTR, OCTAVEBIG, OCTAVESMALL, OCTAVE1, OCTAVE2, OCTAVE3, OCTAVE4, OCTAVE5} teOctave;														

terrPlay PlayNoteFreq(uint8_t note, uint8_t time);

/********************************************************
*   Воспроизведение ноты
*
*********************************************************/

terrPlay PlayNoteFreq(uint8_t notefreq, uint8_t time)
{
	BeepSetTone(notefreq);
	osDelay(time);
}
/************** End function  PlayNoteFreq   ************/

/********************************************************
*   Воспроизведение мелодии 
*
*********************************************************/

terrPlay PlayMelody(tMelody melody)
{
	uint16_t posmelody = 0;
	tMelody pointmelody = melody;
	char notes[10];
	uint16_t lenmelody = strlen(melody);
	if(lenmelody < 2) return playERR;

	uint8_t duration = 0, note, time;
	while(posmelody<lenmelody)
	{
		for(uint8_t i=0; i<10; i++)
			notes[i] = 0;
		sscanf(pointmelody, "%s", notes);
//		pointmelody += strlen(note)+1; 
//		posmelody += strlen(note)+1;
//		// получаем длительность ноты
//		uint32_t duration;
//		sscanf(note, "%u", &duration);
		for(uint8_t i=0; i<strlen(notes); i++)
		{
			
		}
	}
};

/************** End function  PlayMelody ****************/