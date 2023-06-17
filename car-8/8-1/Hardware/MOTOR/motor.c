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

//限制电机脉冲在规定范围内 限幅
void Limit(int *motorA,int* motorB){
	if(*motorA>PWM_MAX)*motorA=PWM_MAX;
	if(*motorA<PWM_MIN)*motorA=PWM_MIN;
	
	if(*motorB>PWM_MAX)*motorB=PWM_MAX;
	if(*motorB<PWM_MIN)*motorB=PWM_MIN;
}

//PID运算完成后最终PWM值  motor1 左轮和右轮 motor2
//正值  正传 多少个脉冲  负值  反转多少个脉冲
void Set_PWM(int motor1,int motor2)
{
	if(motor1>0){	//正转		
		PWM_SetCompare1(abs(motor1)+Dead_Zone);
		PWM_SetCompare2(0);
	}
	else { 			//反转
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


