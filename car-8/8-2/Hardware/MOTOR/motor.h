#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "sys.h"


void Limit(int *motorA,int* motorB);
void Set_PWM(int motor1,int motor2);
void Stop(float *Med_Jiaodu,float *Jiaodu);
#endif