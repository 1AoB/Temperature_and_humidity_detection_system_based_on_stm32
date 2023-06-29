#include "key.h"
#include "delay.h"
void KEY_Config(void)
{
	//��A�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//PA0����Ϊ��������
	GPIO_InitTypeDef key = {0};
	key.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	key.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&key);
}

uint8_t KEY_GetValue(void)
{
	if(KEY==1)//�жϰ����Ƿ���
	{
		Delay_ms(20);//����
		if(KEY==1)//�ٴ��жϰ����Ƿ���
		{
			while(KEY==1);//���ּ��
			return 1;
		}
	}
	return 0;
}
	
