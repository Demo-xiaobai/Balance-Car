#include "Blue.h"
#include "OLED.h"
#include "sys.h"

void Blue_UART3_Init(){
	
	GPIO_InitTypeDef GPIO_InitStruct1,GPIO_InitStruct2;
	USART_InitTypeDef USART_InitStructer;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_AF_PP;  //B10 tx 复用推挽输出 
	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct1);
	
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//b11 rx  浮空输入
	GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

	
	USART_InitStructer.USART_BaudRate = 9600;	//串口波特率
	USART_InitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStructer.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_InitStructer.USART_Parity = USART_Parity_No;		//无奇偶校验位
	USART_InitStructer.USART_StopBits = USART_StopBits_1;	//停止位1 位
	USART_InitStructer.USART_WordLength = USART_WordLength_8b;	//字长8位数据格式
	
	USART_Init(USART3,&USART_InitStructer);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3,ENABLE);
}

//蓝牙接收数据
/*
0x00:刹车
0x01:前进
0x05:后退
0x03:左转
0x07:右转

*/
u8 Fore,Back,Left,Right;
void USART3_IRQHandler(void) {
	int Blue_Data;
	OLED_ShowChar(2,3,'F');
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET){
		Blue_Data = USART_ReceiveData(USART3);		//读取接收到的数据
		switch(Blue_Data){
			case 0x00:
				Fore = 0,Back = 0,Left = 0,Right = 0;break;
			case 0x01:
				Fore = 1,Back = 0,Left = 0,Right = 0;break;
			case 0x05:
				Fore = 0,Back = 1,Left = 0,Right = 0;break;
			case 0x03:
				Fore = 0,Back = 0,Left = 1,Right = 0;break;
			case 0x07:
				Fore = 0,Back = 0,Left = 0,Right = 1;break;
			default:
				Fore = 0,Back = 0,Left = 0,Right = 0;break;
		}
	}
}

//一个
void USART3_Send_Data(char data)
{
	USART_SendData(USART3,data);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
}

//一串
void USART3_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART3_Send_Data(*String++);
	}
}