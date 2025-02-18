#include "servo_motor.h"

#define SIGN 1					//�����ת��
#define MAX_ANGLE 60			//���ת���

void Set_Angle(float angle)
{
	if(angle>MAX_ANGLE)angle=MAX_ANGLE;
	else if(angle<-MAX_ANGLE)angle=-MAX_ANGLE;
	TIM_SetCompare1(TIM1,710+SIGN*angle*50/9);
}

//������ת�䣨�޺󳷣�
void Turn_Left_90(void)
{
	Set_Angle(-60);
	delay_ms(10);		//��ֹûת����С���Ϳ�ʼ��ʻ
	Dc_Motor_Move(23,30);
	Set_Angle(0);
	Beep_Times(3,1);
}

void Turn_Right_90(void)
{
	Set_Angle(60);
	delay_ms(10);		//��ֹûת����С���Ϳ�ʼ��ʻ
	Dc_Motor_Move(52,30);
	Set_Angle(0);
	Beep_Times(3,1);
}


//�̾���ת�䣨�к󳷣�
void Fast_Turn_Left_90(void)
{
	Set_Angle(-60);
	delay_ms(10);		//��ֹûת����С���Ϳ�ʼ��ʻ
	Dc_Motor_Move(15,50);
	
	Set_Angle(0);
	delay_ms(10);
	
	Set_Angle(60);
	delay_ms(10);		//��ֹûת����С���Ϳ�ʼ��ʻ
	Dc_Motor_Move(-23,30);
	
	Set_Angle(0);
	Beep_Times(3,1);
}

void Fast_Turn_Right_90(void)
{
	Set_Angle(60);
	delay_ms(20);		//��ֹûת����С���Ϳ�ʼ��ʻ
	Dc_Motor_Move(30,30);
	
	Set_Angle(0);
	delay_ms(10);
	
	Set_Angle(-60);
	delay_ms(10);		//��ֹûת����С���Ϳ�ʼ��ʻ
	Dc_Motor_Move(-14.5,50);
	
	Set_Angle(0);
	Beep_Times(3,1);
}
