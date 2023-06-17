#include "encoder.h"
#include "motor.h"
#include "OLED.h"
#include "ANO.h"

/*
	ǰ���޸������ٶ�

*/

float Med_Angle=3;//��е��ֵ--��ʹ��С������ƽ��ס�ĽǶȡ   �
float taget_Speed = 0;
float
	Vertical_Kp=121.2,	//ֱ����KP��KD
	Vertical_Kd=0.24;
float
	Velocity_Kp=-0.09,	//�ٶȻ�KP��KI   -0.09
	Velocity_Ki=-0.00045;	//-0.00045
float 
	Turn_Kp=-0.1;			//-0.1

int Vertical_out,Velocity_out=0,Turn_out=0;		//ֱ����&�ٶȻ�&ת�� ���������

int Vertical(float Med,float Angle,float gyro_Y);//��������
int Velocity(int encoder_left,int encoder_right);
int Turn(int gyro_Z);


void EXTI4_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line4)==SET){
		int PWM_out;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){
			EXTI_ClearITPendingBit(EXTI_Line4);
			Encoder_Left = Read_Speed(2);
			Encoder_Right = -Read_Speed(4);;//��������	
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ��������� ���ٶ�
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ����������� ���ٶ�
			
			//2.������ѹ��ջ������У�����������������
			Vertical_out=Vertical(Med_Angle,Pitch,gyroy);				//ֱ����
			Velocity_out=Velocity(Encoder_Left,Encoder_Right);	//�ٶȻ�
//			Turn_out=Turn(gyroz);																//ת��
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//�������
			//3.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
			MOTO1=PWM_out-Turn_out;//����
			MOTO2=PWM_out+Turn_out;//�ҵ��
			Limit(&MOTO1,&MOTO2);//PWM�޷�			
			Set_PWM(MOTO1,MOTO2);//���ص�����ϡ�
//			SendF3(MOTO1,MOTO2,0);    
		}
	}
}

/*********************
ֱ����PD��������Kp*Ek+Kd*Ek_D

��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�     Vertical_out=Vertical(Med_Angle = 0,Pitch,gyroy);
���ڣ�ֱ�������
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	PWM_out=Vertical_Kp*Angle+Vertical_Kd*(gyro_Y-0);//��1��
	return PWM_out;
}

/*********************
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//��2��
	float a=0.7;//��3��
	
	//1.�����ٶ�ƫ��
	Encoder_Err=(encoder_left+encoder_right)-0;//��ȥ���
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;//��4��
	//4.�����޷�
	Encoder_S=Encoder_S>20000?20000:(Encoder_S<(-20000)?(-20000):Encoder_S);
	//5.�ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;//��5��
	return PWM_out;
}

/*********************
ת�򻷣�ϵ��*Z����ٶ�
*********************/
int Turn(int gyro_Z)
{
	int PWM_out;
	
	PWM_out=Turn_Kp*gyro_Z;
	return PWM_out;
}