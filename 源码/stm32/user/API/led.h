#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

#define LED1_ON     GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_RESET)
#define LED1_OFF    GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_SET)
#define LED1_TOGGLE GPIOE->ODR ^= 1<<2

#define LED2_ON     GPIO_WriteBit(GPIOE,GPIO_Pin_3,Bit_RESET)
#define LED2_OFF    GPIO_WriteBit(GPIOE,GPIO_Pin_3,Bit_SET)
#define LED2_TOGGLE GPIOE->ODR ^= 1<<3

#define LED3_ON     GPIO_WriteBit(GPIOE,GPIO_Pin_4,Bit_RESET)
#define LED3_OFF    GPIO_WriteBit(GPIOE,GPIO_Pin_4,Bit_SET)
#define LED3_TOGGLE GPIOE->ODR ^= 1<<4

#define LED4_ON     GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_RESET)
#define LED4_OFF    GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_SET)
#define LED4_TOGGLE GPIOE->ODR ^= 1<<5

void LED_Config(void);
void led_ls(void);
void led_breath(void);

#endif
