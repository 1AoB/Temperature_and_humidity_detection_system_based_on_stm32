#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f10x.h"

#define WIFI_ENABLE			GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define WIFI_DISABLE		GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define MY_SSID					"CC"//热点名，不能出现中文
#define MY_PASSWORD			"wxn12345678"//热点密码，不能出现中文
#define MY_IP						"www.bemfa.com"
#define MY_PORT					8344
typedef struct
{
	u8 RX_buff[1032];//保存接收到的数据
	u16 RX_count;//记录接收数据的数量
	vu8 USART_RevOverflag;//接收结束的标志
}USART3_DataStruct;
extern USART3_DataStruct USART3_Data;


void usart3_config(void);
void USART3_SendData(u8 data);
void USART3_SendStr(char * p);
void USART3_SendArray(u8 Arr[] ,u16 DataLenth);
char * FindStr(char* dest, char* src, uint32_t timeout);
void Exit_Scotch(void);
uint8_t ESP8266_SendCmd_RecAck(char *cmd, char *ack, uint32_t timeout, uint8_t check_cnt);
u8 Connect_AP(char *SSID,char *PASSWORD);
u8 Connect_Server(char *IP,u16 PORT);
void WIFI_Iinit(void);

#endif
