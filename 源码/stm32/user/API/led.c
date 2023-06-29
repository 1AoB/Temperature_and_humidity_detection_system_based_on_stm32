#include "led.h"
#include "delay.h"
void LED_Config(void)
{
	//打开E端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	//PE2配置为通用推挽输出
	GPIO_InitTypeDef led = {0};
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	led.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE,&led);
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}
void led_ls(void)
{
	static uint8_t state = 0;
	switch(state)
	{
		case 0:LED1_ON;LED2_OFF;LED3_OFF;LED4_OFF;break;
		case 1:LED1_OFF;LED2_ON;LED3_OFF;LED4_OFF;break;
		case 2:LED1_OFF;LED2_OFF;LED3_ON;LED4_OFF;break;
		case 3:LED1_OFF;LED2_OFF;LED3_OFF;LED4_ON;break;
	}
	state++;
	state%=4;//state = state%4  
}
void led_breath(void)
{
	uint32_t t = 0;
	for(t=0;t<2000;t+=5)
	{
		LED1_ON;
		Delay_us(t);
		LED1_OFF;
		Delay_us(2000-t);
	}
	for(t=0;t<2000;t+=5)
	{
		LED1_ON;
		Delay_us(2000-t);
		LED1_OFF;
		Delay_us(t);
	}
}
	
