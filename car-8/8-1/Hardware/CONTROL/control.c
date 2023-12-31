#include "encoder.h"
#include "motor.h"
#include "OLED.h"
#include "ANO.h"

/*
	前进修改期望速度

*/

float Med_Angle=3;//机械中值--能使得小车真正平衡住的角度�   �
float taget_Speed = 0;
float
	Vertical_Kp=121.2,	//直立环KP、KD
	Vertical_Kd=0.24;
float
	Velocity_Kp=-0.09,	//速度环KP、KI   -0.09
	Velocity_Ki=-0.00045;	//-0.00045
float 
	Turn_Kp=-0.1;			//-0.1

int Vertical_out,Velocity_out=0,Turn_out=0;		//直立环&速度环&转向环 的输出变量

int Vertical(float Med,float Angle,float gyro_Y);//函数声明
int Velocity(int encoder_left,int encoder_right);
int Turn(int gyro_Z);


void EXTI4_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line4)==SET){
		int PWM_out;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){
			EXTI_ClearITPendingBit(EXTI_Line4);
			Encoder_Left = Read_Speed(2);
			Encoder_Right = -Read_Speed(4);;//读编码器	
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据 加速度
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据 角速度
			
			//2.将数据压入闭环控制中，计算出控制输出量。
			Vertical_out=Vertical(Med_Angle,Pitch,gyroy);				//直立环
			Velocity_out=Velocity(Encoder_Left,Encoder_Right);	//速度环
//			Turn_out=Turn(gyroz);																//转向环
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//最终输出
			//3.把控制输出量加载到电机上，完成最终的的控制。
			MOTO1=PWM_out-Turn_out;//左电机
			MOTO2=PWM_out+Turn_out;//右电机
			Limit(&MOTO1,&MOTO2);//PWM限幅			
			Set_PWM(MOTO1,MOTO2);//加载到电机上。
//			SendF3(MOTO1,MOTO2,0);    
		}
	}
}

/*********************
直立环PD控制器：Kp*Ek+Kd*Ek_D

入口：期望角度、真实角度、真实角速度     Vertical_out=Vertical(Med_Angle = 0,Pitch,gyroy);
出口：直立环输出
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	PWM_out=Vertical_Kp*Angle+Vertical_Kd*(gyro_Y-0);//【1】
	return PWM_out;
}

/*********************
速度环PI：Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//【2】
	float a=0.7;//【3】
	
	//1.计算速度偏差
	Encoder_Err=(encoder_left+encoder_right)-0;//舍去误差
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;//【4】
	//4.积分限幅
	Encoder_S=Encoder_S>20000?20000:(Encoder_S<(-20000)?(-20000):Encoder_S);
	//5.速度环控制输出计算
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;//【5】
	return PWM_out;
}

/*********************
转向环：系数*Z轴角速度
*********************/
int Turn(int gyro_Z)
{
	int PWM_out;
	
	PWM_out=Turn_Kp*gyro_Z;
	return PWM_out;
}