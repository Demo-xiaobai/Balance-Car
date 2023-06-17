//数据发送的缓冲区
#include "stm32f10x.h"                  // Device header
#include "uart2.h"

//多字节数据拆分成单字节的数据
#define BYTE0(dwTemp)       (*(char *)(&dwTemp)) //取出int型变量的低字节
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1)) //取存储在此变量下一内存字节的内容，高字节
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

u8 DataToSend[100];
//样例   char 1 8bit ，int 4 32bit，
void SendF1(s16 _a,s16 _b ,s32 _c)
{
	//发送的数据填到发送缓冲区中
	u8 _cnt = 0;
	u8 i;
		//校验的计算
	u8 sc = 0;	//和校验
	u8 ac = 0;	//附加校验
	DataToSend[_cnt++] = 0xAA;//帧头
	DataToSend[_cnt++] = 0xFF;//广播
	DataToSend[_cnt++] = 0xF1;//功能码
	DataToSend[_cnt++] = 8;//数据长度 2+2+4 = 8

	//填充数据   把数据转换成小端模式
	DataToSend[_cnt++] = BYTE0(_a);
	DataToSend[_cnt++] = BYTE1(_a);
	
	DataToSend[_cnt++] = BYTE0(_b);
	DataToSend[_cnt++] = BYTE1(_b);
	
	DataToSend[_cnt++] = BYTE0(_c);
	DataToSend[_cnt++] = BYTE1(_c);
	DataToSend[_cnt++] = BYTE2(_c);
	DataToSend[_cnt++] = BYTE3(_c);
	

	for(i = 0;i<DataToSend[3]+4;i++){  
	//DataToSend[3] = 8 就已知了数据区的长度 在加上帧头 广播 功能码4个字节的长度                 8+4=12   对数据循环累加
		sc+=DataToSend[i];
		ac += sc;
	}
	//放入校验的数据
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	//最后发送数据
	SendArrayData(DataToSend,_cnt);
	
}

//飞控协议
void Send03(s16 ROL,s16 PIT,s16 YAW,u8 FUSION_STA)
{
	//发送的数据填到发送缓冲区中
	u8 i;
	u8 _cnt = 0;
	u8 sc = 0;	//和校验
	u8 ac = 0;	//附加校验
	DataToSend[_cnt++] = 0xAA;//帧头
	DataToSend[_cnt++] = 0xFF;//广播
	DataToSend[_cnt++] = 0x03;//功能码
	DataToSend[_cnt++] = 7;//数据长度 

	//填充数据   把数据转换成小端模式
	DataToSend[_cnt++] = BYTE0(ROL);
	DataToSend[_cnt++] = BYTE1(ROL);

	
	DataToSend[_cnt++] = BYTE0(PIT);
	DataToSend[_cnt++] = BYTE1(PIT);

	
	DataToSend[_cnt++] = BYTE0(YAW);
	DataToSend[_cnt++] = BYTE1(YAW);

	
	DataToSend[_cnt++] = FUSION_STA;
	
	//校验的计算

	for(i = 0;i<DataToSend[3]+4;i++){  //8+4=12   对数据循环累加
		sc+=DataToSend[i];
		ac += sc;
	}
	//放入校验的数据
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
		
	//最后发送数据
	SendArrayData(DataToSend,_cnt);
}
void SendF2(uint8_t speed)
{
		//发送的数据填到发送缓冲区中
	u8 _cnt = 0;
	u8 i;
		//校验的计算
	u8 sc = 0;	//和校验
	u8 ac = 0;	//附加校验
	DataToSend[_cnt++] = 0xAA;//帧头
	DataToSend[_cnt++] = 0xFF;//广播
	DataToSend[_cnt++] = 0xF1;//功能码
	DataToSend[_cnt++] = 1;//数据长度 2+2+4 = 8

	//填充数据   把数据转换成小端模式
	DataToSend[_cnt++] = speed;
	
	//校验的计算
	for(i = 0;i<DataToSend[3]+4;i++){  //8+4=12   对数据循环累加
		sc+=DataToSend[i];
		ac += sc;
	}
	//放入校验的数据
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
		
	//最后发送数据
	SendArrayData(DataToSend,_cnt);
}
//pid 波形 F3
void SendF3(s32 balance,s32 v ,s32 turn)
{
	//发送的数据填到发送缓冲区中
	u8 _cnt = 0;
	u8 i;
		//校验的计算
	u8 sc = 0;	//和校验
	u8 ac = 0;	//附加校验
	DataToSend[_cnt++] = 0xAA;//帧头
	DataToSend[_cnt++] = 0xFF;//广播
	DataToSend[_cnt++] = 0xF3;//功能码
	DataToSend[_cnt++] = 12;//数据长度 2+2+4 = 8

	//填充数据   把数据转换成小端模式
	DataToSend[_cnt++] = BYTE0(balance);
	DataToSend[_cnt++] = BYTE1(balance);
	DataToSend[_cnt++] = BYTE2(balance);
	DataToSend[_cnt++] = BYTE3(balance);
	
	DataToSend[_cnt++] = BYTE0(v);
	DataToSend[_cnt++] = BYTE1(v);
	DataToSend[_cnt++] = BYTE2(v);
	DataToSend[_cnt++] = BYTE3(v);
	
	DataToSend[_cnt++] = BYTE0(turn);
	DataToSend[_cnt++] = BYTE1(turn);
	DataToSend[_cnt++] = BYTE2(turn);
	DataToSend[_cnt++] = BYTE3(turn);
	

	for(i = 0;i<DataToSend[3]+4;i++){  
	//DataToSend[3] = 8 就已知了数据区的长度 在加上帧头 广播 功能码4个字节的长度                 8+4=12   对数据循环累加
		sc+=DataToSend[i];
		ac += sc;
	}
	//放入校验的数据
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	//最后发送数据
	SendArrayData(DataToSend,_cnt);
	
}
