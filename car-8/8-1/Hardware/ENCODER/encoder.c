#include "stm32f10x.h"                  // Device header
#include "encoder.h"

/*
		AC1----A1		TIM2_CH2	
		AC2----A0		TIM2_CH1
		BC1----B7       TIM4_CH2
		BC2----B6		TIM4_CH1
*/
void Encoder_TIM2_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //������������ �ø�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
//	��ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructer);
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;		//ָ��ʱ�Ӳ���Ƶ, �źŲ���
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����ϼ���
	TIM_TimeBaseInitStructer.TIM_Period = 65535;	//�Զ���װֵ arr
	TIM_TimeBaseInitStructer.TIM_Prescaler = 0;		//����Ƶ
	TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);	//�߼���ʱ��
	
	//������ͨ��
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	
	TIM_ICInitStruct.TIM_ICFilter = 10;	//�˲���
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	
	//����жϱ�־λ  ���ʱ�����߹����һ��
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM2,0);		//
	
	TIM_Cmd(TIM2,ENABLE);

}
//�������ĳ�ʼ��
void Encoder_TIM4_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //������������ �ø�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
//	��ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructer);
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;		//ָ��ʱ�Ӳ���Ƶ, �źŲ���
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����ϼ���
	TIM_TimeBaseInitStructer.TIM_Period = 65535;	//�Զ���װֵ arr
	TIM_TimeBaseInitStructer.TIM_Prescaler = 0;		//����Ƶ
	TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructer);	//�߼���ʱ��
	
	//������ͨ��
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	
	TIM_ICInitStruct.TIM_ICFilter = 10;	//�˲���
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	//����жϱ�־λ  ���ʱ�����߹����һ��
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM4,0);		//
	
	TIM_Cmd(TIM4,ENABLE);

}

//��ȡ�ٶ�
int Read_Speed(int TIMx){
	int value_1;
	switch(TIMx){
		case 2:
			value_1 = (short)TIM_GetCounter(TIM2);
			TIM_SetCounter(TIM2,0);
			break;
		case 4:
			value_1 = (short)TIM_GetCounter(TIM4);
			TIM_SetCounter(TIM4,0);
			break;
		default:value_1 = 0;
	}
	return value_1;
}
//�жϺ���  Ѱַ
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

void TIM4_IRQHandler(){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
