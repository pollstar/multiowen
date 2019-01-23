#include "term.h"
#include "adc.h"
#include <stdint.h>

uint16_t termADCValue[2];		// значение АЦП терморезистора

typedef struct sACDToTemp{
	uint16_t Temperature;
	uint16_t ADCValue;
} tsADCToTemp;


#define TEMP_DATA_NUMBER	30
tsADCToTemp vACDToTemp[TEMP_DATA_NUMBER] = {{10,50},{15,150},{30,430},{40,600},{50,850},{60,1100},{70,1355},{85,1790},{95,2110},\
																					{100,2240},{110,2550},{115,2650},{120,2780},{130,3000},{140,3200},{152,3400},{155,3450},\
																					{160,3540},{172,3680},{175,3790},{188,3860},{201,3930},{255,5000}};

void TERM_Init(void)
{
  MX_ADC1_Init();
	  /* Run the ADC calibration */  
  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
  {
    /* Calibration Error */
//    Error_Handler();
  }
  /* Start ADC conversion on regular group with transfer by DMA */
	if (HAL_ADCEx_InjectedStart_IT(&hadc1) != HAL_OK)
  {
    /* Start Error */
//    Error_Handler();
  }
};


void TERM_DeInit(void)
{
};
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  termADCValue[TermDown] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
  termADCValue[TermUp] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
};

/*************************************************************
*		
*
*
**************************************************************/

uint8_t TERM_GetTemp(teTerm t)
{
	/*--- проверяем на правильность показаний термодатчика ----*/
	if((termADCValue[t] < vACDToTemp[0].ADCValue))
		return 0;
	if(TEMP_DATA_NUMBER < 2)
		return 0;
//	down.ADCValue = vACDToTemp[0].ADCValue;
	/*--- подбираем диапазон для оверсемпоинга ---*/
	uint8_t i = 1;
	while((i<TEMP_DATA_NUMBER)&&(termADCValue[t] > vACDToTemp[i].ADCValue))
		i++;
	/*--- если значение ADC выше значений в таблицеЮ вывод ошибки ---*/
	if(i>=TEMP_DATA_NUMBER)
		return 0;
	/*--- рассчитываем значение температуры на участке семпоинга ----*/
	uint32_t res = ((vACDToTemp[i].ADCValue - vACDToTemp[i-1].ADCValue)*10) / (vACDToTemp[i].Temperature - vACDToTemp[i-1].Temperature);
	res = (((termADCValue[t]-vACDToTemp[i-1].ADCValue)*10)/res)+vACDToTemp[i-1].Temperature;
	return (uint8_t)(res);
/** End TERM_GetTemp *******************************************/

};

