#ifndef __PWM_H__
#define __PWM_H__
#include "sys.h"

void PWM_Init_TIM3(u16 Psc,u16 Per);
void PWM_SetCompare1(int Compare);
void PWM_SetCompare2(int Compare);
void PWM_SetCompare3(int Compare);
void PWM_SetCompare4(int Compare);


#endif