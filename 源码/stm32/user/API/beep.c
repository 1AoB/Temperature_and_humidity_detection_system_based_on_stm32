#include "beep.h"
void BEEP_Config(void)
{
	//��C�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//PC0����Ϊͨ���������
	GPIO_InitTypeDef beep = {0};
	beep.GPIO_Mode = GPIO_Mode_Out_PP;
	beep.GPIO_Pin = GPIO_Pin_0;
	beep.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&beep);
	BEEP_OFF;
}
	
