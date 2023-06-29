#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x.h"
#define BEEP_ON     GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET)
#define BEEP_OFF    GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET)
#define BEEP_TOGGLE GPIOC->ODR ^= 1<<0


void BEEP_Config(void);

#endif
