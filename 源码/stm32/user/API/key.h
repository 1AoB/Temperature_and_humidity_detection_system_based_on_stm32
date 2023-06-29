#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"
#define KEY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

void KEY_Config(void);
uint8_t KEY_GetValue(void);

#endif
