#include "motor.h"
#include "PWM.h"
#include "stdlib.h"
/*
		AIN1------A6	TIM3_CH1
		AIN2------A7	TIM3_CH2
		BIN1------B0	TIM3_CH3
		BIN2------B1	TIM3_CH4
*/
int Dead_Zone=3800;

//���Ƶ�������ڹ涨��Χ�� �޷�
void Limit(int *motorA,int* motorB){
	if(*motorA>PWM_MAX)*motorA=PWM_MAX;
	if(*motorA<PWM_MIN)*motorA=PWM_MIN;
	
	if(*motorB>PWM_MAX)*motorB=PWM_MAX;
	if(*motorB<PWM_MIN)*motorB=PWM_MIN;
}

//PID������ɺ�����PWMֵ  motor1 ���ֺ����� motor2
//��ֵ  ���� ���ٸ�����  ��ֵ  ��ת���ٸ�����
void Set_PWM(int motor1,int motor2)
{
	if(motor1>0){	//��ת		
		PWM_SetCompare1(abs(motor1)+Dead_Zone);
		PWM_SetCompare2(0);
	}
	else { 			//��ת
		PWM_SetCompare1(0);
		PWM_SetCompare2(abs(motor1)+Dead_Zone);
	}
	
	if(motor2>0){	
		PWM_SetCompare3(abs(motor2)+Dead_Zone);
		PWM_SetCompare4(0);
	}
	else {	 
		PWM_SetCompare3(0);		
		PWM_SetCompare4(abs(motor2)+Dead_Zone);
	}
}


