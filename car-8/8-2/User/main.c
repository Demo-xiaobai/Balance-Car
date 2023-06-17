#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "encoder.h"
#include "PWM.h"
#include "exti.h"
#include "control.h"
#include "motor.h"
#include "uart2.h"
#include "ANO.h"
#include "sys.h"
#include "Blue.h"


int PWM_MAX = 7200,PWM_MIN = -7200;					//积分限幅
int MOTO1,MOTO2;				
float Pitch,Roll,Yaw;					//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;					//加速度
int Encoder_Left,Encoder_Right;			

int main(void)
{

	OLED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 
	uart1_init(115200);	 	
	delay_init();
	MPU_Init();	
	UART2_Init();	//串口输出PID波形   或者 欧拉角
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	PWM_Init_TIM3(0,7199);
	mpu_dmp_init();		
	MPU6050_EXTI_Init();
	Blue_UART3_Init();
	OLED_ShowChar(1,2,'F');
	while (1)
	{
//		
		if(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)==0)	
		{
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	
		}

		
	
		OLED_ShowString(2,2,"Pitch:");
		OLED_ShowString(3,2,"Roll");
		OLED_ShowString(4,2,"yaw");
		
		OLED_ShowSignedNum(2,9,Pitch,5);
		OLED_ShowSignedNum(3,9,Roll,5);
		OLED_ShowSignedNum(4,9,Yaw,5);
		
	}
}
