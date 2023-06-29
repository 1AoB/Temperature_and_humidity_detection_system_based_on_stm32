#ifndef	__ADC_H
#define __ADC_H

#include "stm32f10x.h"

extern u16 adc_data[2];

void ADC_Config(void);
u16 Get_HUMValue(void);
u16 Get_RainValue(void);
#endif
