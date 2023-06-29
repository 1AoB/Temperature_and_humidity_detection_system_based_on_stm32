#include "delay.h"
uint32_t timeled[2] = {0,500};
uint32_t timekey[2] = {0,10};
uint32_t timeadc[2] = {0,2000};

//系统滴答定时器的中断函数
//1ms
void SysTick_Handler(void)
{
	timeled[0]++;
	timekey[0]++;
	timeadc[0]++;

}




//系统定时器初始化
void Systick_Init(uint32_t load)
{
	if(SysTick_Config(load) == 1)
	{
		while(1);
	}
}


void Delay_us(uint32_t time)
{
	while(time--) {
		delay_1us();
	}
}

void Delay_ms(uint32_t time)
{
	uint64_t t = time*1000;
	while(t--) {
		delay_1us();
	}
}






