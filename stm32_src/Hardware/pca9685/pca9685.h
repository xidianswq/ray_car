#ifndef __PCA9685_H
#define __PCA9685_H
#include "public.h"
#include "stdio.h"
#include "math.h"  
#include "led.h"
#include "pid.h"
#include "usart.h"

#define PCA9685_ADDRESS 0x80	//1+A5+A4+A3+A2+A1+A0+w/r 

#define PCA9685_SUBADR1 0x02
#define PCA9685_SUBADR2 0x03
#define PCA9685_SUBADR3 0x04

#define PCA9685_MODE1    0x00   //MODE1�Ĵ�����ַ
#define PCA9685_PRESCALE 0xFE   //PRE_SCALE�Ĵ�����ַ

#define LED0_ON_L  0x06			//ͨ��0���ĸ����ƼĴ���
#define LED0_ON_H  0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

#define ALLLED_ON_L  0xFA		//ȫ��ͨ�����ĸ����ƼĴ���
#define ALLLED_ON_H  0xFB		//�������ʱһ�㲻��
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define SERVO000	130 		//0�ȶ�Ӧ4096���������ֵ
#define SERVO090    325			//90�ȶ�Ӧ4096���������ֵ
#define SERVO180	520 		//180�ȶ�Ӧ4096���������ֵ
#define SERVO_L_RANGE	100		//����ˮƽ��ת��Χ
#define SERVO_V_RANGE   150	 	//���ƴ�ֱ��ת��Χ
#define SERVOSUB	390			//0�ȵ�90�ȵ�4096�����		
#define SERVOADJUST 325			//������ֵʹˮƽ(��װʱ���)

extern int Off_L;				//ˮƽ�����ǰ�趨pwm��offֵ
extern int Off_V;				//��ֱ�����ǰ�趨pwm��offֵ

void PCA9685_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void PCA9685_WriteReg(uint8_t RegAddress, uint8_t Data);
void PCA9685_Reset(void);
void PCA9685_SetPwmFreq(float freq);
void PCA9685_SetPwm(int num, int on, int off);
void PCA9685_Init(void);
void Spinnig_Level(int diff);
void Spinnig_Vert(int diff);
void Set_Angle_Vert(float angle);
void Follow_Point(void);
int Get_Point_Pos(void);
void JoyStick_Control(void);

#endif
