#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "bsp_lcd.h"
#include "adc.h"
#include "math.h"
#include "usart1.h"
#include <stdio.h>
#include "esp8266.h"
#include "string.h"

//厂家测试接口
void JTAG_SW_Config(void)
{
	//关闭JTAG接口，开启SW接口--PA15、PB3、PB4可用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}


  extern uint32_t timeled[2];
	extern uint32_t timekey[2];
	extern uint32_t timeadc[2];
	extern u8 adc_RAIN_buff[100];
	extern u8 adc_HUM_buff[100];
	extern const unsigned char gImage_3[153600];

void fengmingqi(){
	BEEP_ON;
	Delay_ms(1000);
	BEEP_OFF;
}
void bao_jing(u16 turang,u16 yuliang){

		if(turang <2700)fengmingqi();
		Delay_ms(1000);
		if(yuliang <2000)fengmingqi();
}

int main(void)
{
	uint8_t key = 0;
	uint8_t names = 0;
	char bafa_buf[300];
	JTAG_SW_Config();
	Systick_Init(72000);
	LED_Config();//LED初始化
	BEEP_Config();//蜂鸣器初始化
	KEY_Config();//按键初始化
	TFTLCD_Init();//屏幕初始化
	ADC_Config();//湿度传感器初始化
	USART1_Config();//串口初始化
	printf("串口初始化成功\r\n");
	WIFI_Iinit();
	Connect_Server(MY_IP,MY_PORT);
	LCD_Clear(BLUE);
  POINT_COLOR =BLACK;
LCD_ShowPicture(0,0,240,320,(u8*)gImage_3);
 LCD_Draw_Circle(120,160,120);
 LCD_ShowStringAndHz(0,0,240,320,16,(u8*)"何鑫红",0);
 LCD_ShowStringAndHz(0,16,240,320,16,(u8*)"吕海玲",0);
 LCD_ShowStringAndHz(0,32,240,320,16,(u8*)"陈赜",0);
 LCD_ShowStringAndHz(0,48,240,320,16,(u8*)"武晓宁",0);
 
 //Connect_AP("CC","wxn12345678");

while (1)
  {     
		
		/*if(USART1_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
			{
		}*/
			GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_RESET);
			LED1_ON;
			Delay_ms(500);
			GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_SET);
			LED1_OFF;
			
			led_ls();
		  Delay_ms(500);
			key = KEY_GetValue();
			if(key==1)
		{
			//BEEP_TOGGLE;
			LED1_TOGGLE;
		}
		if(timeled[0]>timeled[1])//经过了500ms
		{
			led_ls();
			timeled[0] = 0;
		}
		if(timekey[0]>timekey[1])
		{
			key = KEY_GetValue();
			if(key==1)
			{
				names++;
				switch(names)
				{
					case 1: printf("何鑫红");break;
					case 2: printf("吕海玲");break;
					case 3: printf("陈赜");break;
					case 4: printf("武晓宁");names = 0;break;
				}
				//BEEP_TOGGLE;
			}
			timekey[0] = 0;
		}
		if(timeadc[0]>timeadc[1])
		{
			u16 turang = Get_HUMValue();
			sprintf(bafa_buf,"cmd=2&uid=27cbc5896652ba776d36b0ae72c8adfc&topic=soilhum&msg=%d\r\n",adc_data[0]);
			USART3_SendArray((u8*)bafa_buf,strlen(bafa_buf));
			u16 yuliang = Get_RainValue();
			sprintf(bafa_buf,"cmd=2&uid=27cbc5896652ba776d36b0ae72c8adfc&topic=rainvalue&msg=%d\r\n",adc_data[1]);
			USART3_SendArray((u8*)bafa_buf,strlen(bafa_buf));
			
			
			bao_jing(turang,yuliang);
			
			//LCD_ShowString(0,80,240,320,16,adc_HUM_buff);
			//LCD_ShowString(0,100,240,320,16,adc_RAIN_buff);
			
			timeadc[0] = 0;
		}
  }
}
/*void my_show_circle(u16 x,u16 y,u16 r)
{
	u16 x0 = 0,y0 = 0;
	uint32_t i = 0;
	for(i=0;i<360;i++)
	{
		x0 = x + r*cos(3.14*i/180);
		y0 = y + r*sin(3.14*i/180);
		LCD_Fast_DrawPoint(x0,y0,YELLOW);
	}

}	*/
