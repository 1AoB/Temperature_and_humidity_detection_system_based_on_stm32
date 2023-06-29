#include "adc.h"
#include "stdio.h"
#include "bsp_lcd.h"
#include "delay.h"

u16 adc_data[2] = {0};
u8 adc_buff[100] = {0};

void ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	//配置ADC的时钟为12MHZ(不超过14MHZ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//开时钟	GPIOA	ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	//配置GPIO口 PA1 PA2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;//选择引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO口的输出速率
	//初始化GPIO口 GPIOA
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置ADC
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//不使用连续转换
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用硬件流控
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;//待转换通道数量
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//不使用扫描模式
	//初始化ADC
	ADC_Init(ADC1, &ADC_InitStruct);
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//重置校准寄存器
	ADC_ResetCalibration(ADC1);
	//等待校准寄存器重置完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	//校准ADC
	ADC_StartCalibration(ADC1);
	//等待ADC校准完成
	while(ADC_GetCalibrationStatus(ADC1));
}

//获取土壤湿度
u16 Get_HUMValue(void)
{
	//配置转换通道、转换顺序、转换时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	//开始转换
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	//等待转换完成
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
	//获取最近一次ADC转换完成后的值
	adc_data[0] = ADC_GetConversionValue(ADC1);
	sprintf((char*)adc_buff,"土壤湿度:%d",adc_data[0]);
	LCD_ShowStringAndHz(0,80,240,320,16,adc_buff,0);
	return adc_data[0];
}

//获取雨量
u16 Get_RainValue(void)
{
	//配置转换通道、转换顺序、转换时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
	//开始转换
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	//等待转换完成
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	//获取最近一次ADC转换完成后的值
	adc_data[1] = ADC_GetConversionValue(ADC1);
	sprintf((char*)adc_buff,"雨量:%d",adc_data[1]);
	LCD_ShowStringAndHz(0,100,240,320,16,adc_buff,0);
	return adc_data[1];
}






