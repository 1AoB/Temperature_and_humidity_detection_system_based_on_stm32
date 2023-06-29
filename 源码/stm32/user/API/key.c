#include "key.h"
#include "delay.h"
void KEY_Config(void)
{
	//打开A端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//PA0配置为浮空输入
	GPIO_InitTypeDef key = {0};
	key.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	key.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&key);
}

uint8_t KEY_GetValue(void)
{
	if(KEY==1)//判断按键是否按下
	{
		Delay_ms(20);//消抖
		if(KEY==1)//再次判断按键是否按下
		{
			while(KEY==1);//松手检测
			return 1;
		}
	}
	return 0;
}
	
