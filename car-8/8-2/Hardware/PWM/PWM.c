#include "PWM.h"

void PWM_Init_TIM3(u16 Psc,u16 Per){
	GPIO_InitTypeDef GPIO_InitStructA,GPIO_InitStrucB;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitStructA.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructA.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructA);
	
	GPIO_InitStrucB.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStrucB.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStrucB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrucB);
	
	//	��ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructer);
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;		//ָ��ʱ�Ӳ���Ƶ, �źŲ���
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����ϼ���
	TIM_TimeBaseInitStructer.TIM_Period = Per;	//�Զ���װֵ arr
	TIM_TimeBaseInitStructer.TIM_Prescaler = Psc;		//����Ƶ
	TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructer);	
	
	//����Ƚϵĳ�ʼ��
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//	ģʽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);	//	Ԥװ����ʹ��
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);				//��ʱ����arr��Ԥװ�ؼĴ���ʹ��
	TIM_Cmd(TIM3,ENABLE);
}
void PWM_SetCompare1(int Compare)
{
	TIM_SetCompare1(TIM3, Compare);
}
void PWM_SetCompare2(int Compare)
{
	TIM_SetCompare2(TIM3, Compare);
}
void PWM_SetCompare3(int Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}
void PWM_SetCompare4(int Compare)
{
	TIM_SetCompare4(TIM3, Compare);
}