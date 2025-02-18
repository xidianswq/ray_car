#ifndef DC_MOTOR_H
#define DC_MOTOR_H
#include "public.h"
#include "oled.h"
#include "Pwm_2.h"
#include "pid.h"
#include "bluetooth.h"
#include "beep.h"
#include "led.h"
#include "servo_motor.h"

//L298N��4��������ƿ����������˿ڣ�����B9��IN1��IN3��B8��IN2��IN4
#define In1_On GPIO_SetBits(GPIOB,GPIO_Pin_3) 
#define In1_Off	GPIO_ResetBits(GPIOB,GPIO_Pin_3)

#define In2_On GPIO_SetBits(GPIOB,GPIO_Pin_4) 
#define In2_Off	GPIO_ResetBits(GPIOB,GPIO_Pin_4)

//#define In3_On GPIO_SetBits(GPIOB,GPIO_Pin_13) 
//#define In3_Off	GPIO_ResetBits(GPIOB,GPIO_Pin_13)

//#define In4_On GPIO_SetBits(GPIOB,GPIO_Pin_12) 
//#define In4_Off	GPIO_ResetBits(GPIOB,GPIO_Pin_12)

/*
������Ϣ��
ֱ����6.7cm
�ܳ���21.05cm
һȦ������������1568
2cm��������149����65532�ڣ���������ȫ���ã�
���ȣ�0.134mm����������룩
*/
#define PER_PULSE 0.134		//mm
#define GEAR_RATIO 120.62	//���ٱ�
#define PPR 13				//������ÿȦ

void Dc_Motor_Init(void);
void Dc_Motor_Stop(void);
void Dc_Motor_Emerg_Stop(void);
void Dc_Motor_Go(void);
void Dc_Motor_Back(void);
void Dc_Motor_Left(void);
void Dc_Motor_Right(void);
void Dc_Motor_SetDuty(int);
void Dc_Motor_SetSpeed_PID(float Target_Speed_Cm_S);
void Dc_Motor_SetSpeed(float Target_Speed_Cm_S);
void Dc_Motor_Move(float Length_Cm,int Gate);
void Show_DC_Motor_State(void);
void Encoder_Init(void);
int16_t Encoder_Get(void);
float Speed_Get(int Time_S);
void Dc_Motor_Mode(void);			//ң�س�����
void Simple_Aviod_Obstacle(void);	//�򵥱��Ϲ���

#endif
