#include "stm32f10x.h"                  // Device header
#include "uart2.h"

void UART2_Init()
{
	GPIO_InitTypeDef GPIO_InitStructer;
	USART_InitTypeDef USART_InitStructer;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	//A2    TX
	GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructer.GPIO_Pin =GPIO_Pin_2;
	GPIO_InitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);
	
	
	
	
	//初始化USART
	USART_InitStructer.USART_BaudRate = 115200;	//波特率
	USART_InitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制，取值可看定义,这里不使用
	USART_InitStructer.USART_Mode = USART_Mode_Tx;	//接收还是发送模式
	USART_InitStructer.USART_Parity = USART_Parity_No;		//校验位
	USART_InitStructer.USART_StopBits = USART_StopBits_1;	//停止位，选1位停止位
	USART_InitStructer.USART_WordLength = USART_WordLength_8b;	//字长选8位
	USART_Init(USART2,&USART_InitStructer);
	
	USART_Cmd(USART2,ENABLE);
}
void SendData(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
	//为避免数据覆盖，等待数据转移到移位寄存器
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);   //USART_FLAG_TXE 发送寄存器置空，
}
void SendArrayData(uint8_t* ArrayData,uint16_t length)
{
	uint16_t i;
	for(i = 0;i<length;i++)
	{
		SendData(ArrayData[i]);
	}
}
