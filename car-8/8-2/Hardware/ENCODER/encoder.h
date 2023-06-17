#ifndef __ENCODER_H__
#define __ENCODER_H__



void Encoder_TIM2_Init(void);
void Encoder_TIM4_Init(void);
int Read_Speed(int TIMx);
void TIM2_IRQHandler();
void TIM4_IRQHandler();

#endif