#include "encoder.h"
#include "motor.h"
#include "OLED.h"
#include "ANO.h"

/*
	Ç°½øÐÞ¸ÄÆÚÍûËÙ¶È

*/

float Med_Angle=3;//»úÐµÖÐÖµ--ÄÜÊ¹µÃÐ¡³µÕæÕýÆ½ºâ×¡µÄ½Ç¶È¡   £
float taget_Speed = 0;
float
	Vertical_Kp=121.2,	//Ö±Á¢»·KP¡¢KD
	Vertical_Kd=0.24;
float
	Velocity_Kp=-0.09,	//ËÙ¶È»·KP¡¢KI   -0.09
	Velocity_Ki=-0.00045;	//-0.00045
float 
	Turn_Kp=-0.1;			//-0.1

int Vertical_out,Velocity_out=0,Turn_out=0;		//Ö±Á¢»·&ËÙ¶È»·&×ªÏò»· µÄÊä³ö±äÁ¿

int Vertical(float Med,float Angle,float gyro_Y);//º¯ÊýÉùÃ÷
int Velocity(int encoder_left,int encoder_right);
int Turn(int gyro_Z);


void EXTI4_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line4)==SET){
		int PWM_out;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){
			EXTI_ClearITPendingBit(EXTI_Line4);
			Encoder_Left = Read_Speed(2);
			Encoder_Right = -Read_Speed(4);;//¶Á±àÂëÆ÷	
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//µÃµ½¼ÓËÙ¶È´«¸ÐÆ÷Êý¾Ý ¼ÓËÙ¶È
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý ½ÇËÙ¶È
			
			//2.½«Êý¾ÝÑ¹Èë±Õ»·¿ØÖÆÖÐ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
			Vertical_out=Vertical(Med_Angle,Pitch,gyroy);				//Ö±Á¢»·
			Velocity_out=Velocity(Encoder_Left,Encoder_Right);	//ËÙ¶È»·
//			Turn_out=Turn(gyroz);																//×ªÏò»·
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//×îÖÕÊä³ö
			//3.°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
			MOTO1=PWM_out-Turn_out;//×óµç»ú
			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
			Limit(&MOTO1,&MOTO2);//PWMÏÞ·ù			
			Set_PWM(MOTO1,MOTO2);//¼ÓÔØµ½µç»úÉÏ¡£
//			SendF3(MOTO1,MOTO2,0);    
		}
	}
}

/*********************
Ö±Á¢»·PD¿ØÖÆÆ÷£ºKp*Ek+Kd*Ek_D

Èë¿Ú£ºÆÚÍû½Ç¶È¡¢ÕæÊµ½Ç¶È¡¢ÕæÊµ½ÇËÙ¶È     Vertical_out=Vertical(Med_Angle = 0,Pitch,gyroy);
³ö¿Ú£ºÖ±Á¢»·Êä³ö
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	PWM_out=Vertical_Kp*Angle+Vertical_Kd*(gyro_Y-0);//¡¾1¡¿
	return PWM_out;
}

/*********************
ËÙ¶È»·PI£ºKp*Ek+Ki*Ek_S
*********************/
int Velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//¡¾2¡¿
	float a=0.7;//¡¾3¡¿
	
	//1.¼ÆËãËÙ¶ÈÆ«²î
	Encoder_Err=(encoder_left+encoder_right)-0;//ÉáÈ¥Îó²î
	//2.¶ÔËÙ¶ÈÆ«²î½øÐÐµÍÍ¨ÂË²¨
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//Ê¹µÃ²¨ÐÎ¸ü¼ÓÆ½»¬£¬ÂË³ý¸ßÆµ¸ÉÈÅ£¬·ÀÖ¹ËÙ¶ÈÍ»±ä¡£
	EnC_Err_Lowout_last=EnC_Err_Lowout;//·ÀÖ¹ËÙ¶È¹ý´óµÄÓ°ÏìÖ±Á¢»·µÄÕý³£¹¤×÷¡£
	//3.¶ÔËÙ¶ÈÆ«²î»ý·Ö£¬»ý·Ö³öÎ»ÒÆ
	Encoder_S+=EnC_Err_Lowout;//¡¾4¡¿
	//4.»ý·ÖÏÞ·ù
	Encoder_S=Encoder_S>20000?20000:(Encoder_S<(-20000)?(-20000):Encoder_S);
	//5.ËÙ¶È»·¿ØÖÆÊä³ö¼ÆËã
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;//¡¾5¡¿
	return PWM_out;
}

/*********************
×ªÏò»·£ºÏµÊý*ZÖá½ÇËÙ¶È
*********************/
int Turn(int gyro_Z)
{
	int PWM_out;
	
	PWM_out=Turn_Kp*gyro_Z;
	return PWM_out;
}