#include "esp8266.h"
#include "stdio.h"
#include "delay.h"
#include "string.h"
#include "stdlib.h"

USART3_DataStruct USART3_Data={0};

//PB10:TXD   	PB11:RXD
void usart3_config(void)
{
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	//配IO口模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//配usart的工作模式
	USART_InitTypeDef USART3_InitStruct;
	USART3_InitStruct.USART_BaudRate=115200;//波特率
	USART3_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//硬件控制失能
	USART3_InitStruct.USART_WordLength=USART_WordLength_8b;//数据长度
	USART3_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;//接受  发送
	USART3_InitStruct.USART_StopBits=USART_StopBits_1;//1位停止位
	USART3_InitStruct.USART_Parity=USART_Parity_No;//关奇偶校验
	USART_Init(USART3,&USART3_InitStruct);
	USART_Cmd(USART3,ENABLE);//使能串口1
	
	//接收中断配置
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	
	//NVIC配置
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_SetPriority(USART3_IRQn,1);
	
	WIFI_DISABLE;
	Delay_ms(50);
	WIFI_ENABLE;
}

void WIFI_Iinit(void)
{
	usart3_config();
	Delay_ms(2000);
	Connect_AP(MY_SSID,MY_PASSWORD);
}

void USART3_SendData(u8 data)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET)
	{}//等待上一帧数据发送完成
	USART_SendData(USART3,data);//发送数据
}

void USART3_SendStr(char * p)
{
	while(*p != '\0')
	{
		USART3_SendData(*p);
		p++;
	}
}

void USART3_SendArray(u8 Arr[] ,u16 DataLenth)
{
	u16 i=0;
	while(DataLenth --)
	{
		USART3_SendData(Arr[i++]);
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		u8 data = USART3->DR;
		USART1->DR = data;
		USART3_Data.RX_buff[USART3_Data.RX_count]=USART3->DR;
		USART3_Data.RX_count++;
		if(USART3_Data.RX_count > 1032)
			USART3_Data.RX_count=0;
	}
	if(USART_GetITStatus(USART3,USART_IT_IDLE) == SET)
	{
		u8 flag=0;
		flag=USART3->SR;
		flag=USART3->DR;//清中断
		USART3_Data.USART_RevOverflag=1;//接收完成
	}
}

/************************************
字符串查找
参数1：在哪个字符串中查找
参数2：要查找的内容
参数3：超时时间
返回值：查找结果
*************************************/
char * FindStr(char* dest, char* src, uint32_t timeout)
{
	while(timeout-- && (strstr(dest, src) == NULL))
	{
		Delay_ms(1);
	}
	return strstr(dest, src);
}

/**
 * 功能：ESP8266发送指令获取应答
 * 参数：
 *       cmd -- 指令字符串
 *       ack -- 应答字符串
 *       timeout -- 应答溢出时长
 *			 check_cnt -- 循环发送指令的次数（可能模组没反应过来，多发几次）
 * 返回值：0 -- 成功		>0 -- 失败
 */

uint8_t ESP8266_SendCmd_RecAck(char *cmd, char *ack, uint32_t timeout, uint8_t check_cnt)
{
	uint32_t len = strlen((char *)cmd);
	USART3_Data.RX_count = 0;
	memset(USART3_Data.RX_buff, 0, sizeof(USART3_Data.RX_buff));

	do{
		//发送命令
		if(cmd != NULL)
		{
			USART3_SendArray((uint8_t *)cmd, len);
			USART3_SendStr("\r\n");
		}
		//查询应答字符串
		if(ack != NULL)
		{
			if(FindStr((char *)USART3_Data.RX_buff, ack, timeout) != NULL) //等待串口接收应答，并解析
			{
				USART3_Data.RX_count = 0;
				return 0;//接收到正确应答
			}
		}
	}while(--check_cnt);
	return 1;//失败
}

//退出透传模式
void Exit_Scotch(void)
{
		USART3_SendStr("+++");
		Delay_ms(500);
}

/************************************
STA模式 连接2.4GHz的热点
参数1：SSID，热点名
参数2：PASSWORD，热点密码
返回值：
*************************************/
u8 Connect_AP(char *SSID,char *PASSWORD)
{
	
	char sendarr[256]={0};
	Exit_Scotch();//退出传透模式
	if(ESP8266_SendCmd_RecAck("AT","OK",500,2))
	{
		printf("AT 测试失败\r\n");
		return 1;
	}
	else
		printf("AT 测试成功\r\n");
	if(ESP8266_SendCmd_RecAck("AT+CWMODE=1","OK",500,2))
	{
		printf("AT+CWMODE 设置失败\r\n");
		return 2;
	}
	else
		printf("AT+CWMODE 设置成功\r\n");
	sprintf(sendarr,"AT+CWJAP=\"%s\",\"%s\"",SSID,PASSWORD);
	if(ESP8266_SendCmd_RecAck(sendarr,"OK",10000,2))
	{
		printf("AT+CWJAP 设置失败\r\n");
		return 3;
	}
	else
	{
		printf("AT+CWJAP 设置成功\r\n");
		printf("连接热点成功\r\n");
	}
	return 0;
}

/************************************
连接服务器
参数1：IP，服务器域名或IP地址
参数2：PORT，服务器端口号
返回值：
*************************************/
u8 Connect_Server(char *IP,u16 PORT)
{
	char sendarr[256]={0};
	if(ESP8266_SendCmd_RecAck("AT+CWJAP?","OK",5000,2) == 1)
	{
		printf("未检测到连接热点\r\n重连中...\r\n");
		if(Connect_AP(MY_SSID,MY_PASSWORD))
		{
			printf("未检测到热点，退出\r\n");
			return 1;
		}
	}
	sprintf(sendarr,"AT+CIPSTART=\"TCP\",\"%s\",%d",IP,PORT);
	if(ESP8266_SendCmd_RecAck(sendarr,"OK",2000,2) == 1)
	{
		printf("连接服务器失败，退出\r\n");
		return 2;
	}
	if(ESP8266_SendCmd_RecAck("AT+CIPMODE=1","OK",500,2) == 1)
	{
		printf("开启透传模式失败，退出\r\n");
		return 3;
	}
	if(ESP8266_SendCmd_RecAck("AT+CIPSEND",">",500,2) == 1)
	{
		printf("进入透传模式失败，退出\r\n");
		return 4;
	}
	return 0;
}


