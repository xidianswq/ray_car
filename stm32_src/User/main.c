//*********************************************ͷ�ļ�����*********************************************
#include "public.h"
#include "bluetooth.h"
#include "oled.h"
#include "Pwm_1.h"
#include "Pwm_2.h"
#include "dc_motor.h"
#include "servo_motor.h"
#include "AD.h"
#include "string.h"	
#include "Delay.h"
#include "Timer.h"
#include "cs100a.h"
#include "usart.h"
#include "beep.h"
#include "led.h"
#include "ray.h"
#include "pca9685.h"
#include "red_ray.h"

//********************************************����ʹ��˵��********************************************
/*
oled.h				GPIOA PIN0/1
bluetooth.h			GPIOA PIN2/3
AD.h				GPIOA PIN4/5 ADC1_CH4/5
Pwm_2.h				GPIOA PIN6/7 TIM3_CH1/2 20hz
Pwm_1.h				GPIOA PIN8 TIM1_CH1 50hz
usart.h				GPIOA PIN9/10 TX/RX Black/White
dc_motor.h			GPIOB PIN3/4/6/7 TIM4_CH1/2
pca9685.h			GPIOB PIN8/9 I2C_1(SCL/SDA) 
beep.h				GPIOB PIN14
led.h				GPIOB PIN15
Timer.h				TIM2 10hz
*/

//********************************************ȫ�ֱ�������********************************************
extern float Voltage[2];	//ad������ѹֵ[0.3.3]			//ad.c
extern int Duty;			//ֱ�����pwm��ռ�ձ�[0,500]		//dc_motor.c
extern float Speed_Cm_S;	//С���ٶ�cm/s					//dc_motor.c
extern char  USART_RX_INFO[USART_REC_LEN];	//uart��������	//usart.c
extern uint16_t RxPackFlag;					//uart���ձ�־λ	//usart.c
extern int x,y;												//PCA9685.c

int16_t Ax,Ay,Az,Pitch,Roll,Yaw;			//mpu6050���յ�������
int mode=0;					//��ǰ����ģʽ

void system_init(void){
	//********************��ʼ������********************
	BlueToothInit(9600,USART_Parity_No,USART_StopBits_1,USART_WordLength_8b);	//������ʼ��
	OLED_Init();				//oled��ʼ��
	Beep_Init();				//��������ʼ��
	Led_Init();					//led�Ƴ�ʼ��
	Red_Ray_Init();				//��ɫ����Ƴ�ʼ��
	uart_init(115200);			//uart1��ʼ��
	Encoder_Init();				//ֱ�������������ʼ��
	AD_Init();					//ad��ʼ��
//	Timer_Init();				//0.1s��ʱ����ʼ��
	PCA9685_Init();				//PCA9685��ʼ��
	PWM_Init();					//ֱ�����pwm��ʼ��
	TIM1_PWM_Init(9999,143);	//һ����20ms���ֱ���20ms/10000��
	TIM_SetCompare1(TIM1,710);	//����Ƕ�Ϊ90��(1.5ms)
	Dc_Motor_Init();			//ֱ�������ʼ����������������õ�GPIO����
}

//********************************************�������С��********************************************
void ray_cvcar(void){
	while(1)
	{
		//���������źŴ�����
		if(Get_BlueTooth_ReceivePackFlag()==1)
		{
			BlueTooth_TransmitPrintf("%s\r\n",BlueTooth_RxPack);
			mode=BlueTooth_RxPack[0];
			switch(mode){
				case 0:Dc_Motor_Stop();break;
				case 1:Dc_Motor_Go();break;
				case 2:Dc_Motor_SetDuty(100);break;
				case 3:Set_Angle(0);break;
				case 4:Set_Angle(-40);break;
				case 5:Set_Angle(40);break;
				case 6:Red_Ray_On();break;
				case 7:Red_Ray_Off();break;
				default:break;
			}
		}
		
		if(RxPackFlag==1){
			RxPackFlag=0;
			Dc_Motor_Stop();
			Follow_Point();
		}
	}
}

//*********************************************����������*********************************************
int main(void)
{	
	system_init();
	ray_cvcar();
}

//*********************************************�жϺ�������*********************************************
//0.1sһ���ж�(oled״̬ˢ��)	
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{	
		//����״̬��ʾ
		Cs100a_Start();
		Show_Voltage_State();
		Show_DC_Motor_State();	
		Show_Distance();
		
		//uart���մ�����ʾ
		//Get_Point_Pos();
		OLED_ShowNum(3,12,x,3);
		//OLED_ShowString(3,12,USART_RX_INFO);
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	}
}

