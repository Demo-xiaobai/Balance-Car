#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "sys.h"

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
void EXTI4_IRQHandler();
int Turn(int gyro_Z,int RC); 


#endif