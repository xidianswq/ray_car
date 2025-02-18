#include "dc_motor.h"

int Duty=0;				//PWMռ�ձ�
int Direction=0;		//С����ʻ���򣨼�ɲʱ�ã�
float Speed_Cm_S=0;		//С���ٶ�cm/s
//extern int order;
int Sum_Pulse=0;		//��ʻ��������������

void Dc_Motor_Stop(void)
{
	In1_Off;
	In2_Off;
	Direction=0;
	Dc_Motor_SetDuty(0);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);			//��ԭ�������ϼ���ģʽ
//	In3_Off;
//	In4_Off;
}

//����С����ʻ����Ϊǰ��
void Dc_Motor_Go(void)
{
	In1_On;
	In2_Off;
	Direction=1;
//	In3_On;
//	In4_Off;
}

//����С����ʻ����Ϊ����
void Dc_Motor_Back(void)
{
	In1_Off;
	In2_On;
	Direction=-1;
//	In3_Off;
//	In4_On;
}

//void Dc_Motor_Right(void)
//{
//	In1_On;
//	In2_Off;
//	In3_Off;
//	In4_Off;
//}

//void Dc_Motor_Left(void)
//{
//	In1_Off;
//	In2_Off;
//	In3_On;
//	In4_Off;
//}

//С����ͣ�������ƶ���
void Dc_Motor_Emerg_Stop(void)
{
	//������ʻ�ٶȼ��趨ɲ�����򣨷����ƶ���
	if(Direction>0)Dc_Motor_Back();
	else if(Direction<0)Dc_Motor_Go();

	//����Ŀǰ�ٶ��趨��ɲռ�ձȺ�ʱ��
	Dc_Motor_SetDuty(5*Speed_Cm_S);
	delay_ms(4+((int)Speed_Cm_S/15)%5);
	Dc_Motor_Stop();
}

//���ÿ���С���ٶȵ�PWM����ռ�ձ�
void Dc_Motor_SetDuty(int duty)
{
	int temp=duty;
	if(temp<0)temp=0;
	if(temp>500)temp=500;				//�޷�����
	TIM_SetCompare2(TIM3,temp);
	TIM_SetCompare1(TIM3,temp);
}

//ͨ��PID����С����ʻ�ٶ�
//������ʻ�ٶȵ�PID����
float DC_Motor_Speed_Kp=1;
float DC_Motor_Speed_Ki=0.1;
float DC_Motor_Speed_Kd=5;		//Dֵ���ֹ����
void Dc_Motor_SetSpeed_PID(float Target_Speed_Cm_S)
{
	int time_out=0;
	int right_times=0;
	float target_speed=Target_Speed_Cm_S;
	
	//����С����ʻ����
	if(Target_Speed_Cm_S>0){
		Dc_Motor_Go();
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);		//��ת���������ϼ���
	}
	else if(Target_Speed_Cm_S<0)
	{
		Dc_Motor_Back();
		target_speed=-Target_Speed_Cm_S;
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);		//��ת���������ϼ���
	}
	
	
	int duty=0;
	while(1)
	{
		Led_Times(4,1);
		time_out++;
		
		duty=Duty+Pid_Control(DC_Motor_Speed_Kp,DC_Motor_Speed_Ki,DC_Motor_Speed_Kd,target_speed,Speed_Cm_S,PID_INCREASE);
		Dc_Motor_SetDuty(duty);
		
		if(Speed_Cm_S>target_speed-1&&Speed_Cm_S<target_speed+1)		//�ж��Ƿ�ﵽ�趨�ٶ�
		{
			right_times++;
			if(right_times>=10)break;									//�ж��ٶ��Ƿ��ȶ�
		}
		else right_times=0;
		
		if(time_out>=100)
		{
			BlueTooth_TransmitPrintf("TIME OUT\r\n");
			Beep_Times(3,2);
			break;														//��ʱֱ���˳�����ֹ���ʲ��㿨��
		}
	}
	Beep_Times(3,1);
}

//���㷨����С����ʻ�ٶ�
void Dc_Motor_SetSpeed(float Target_Speed_Cm_S)
{
	int time_out=0;
	int right_times=0;
	float target_speed=Target_Speed_Cm_S;
	
	//����С����ʻ����
	if(Target_Speed_Cm_S>0){
		Dc_Motor_Go();
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);		//��ת���������ϼ���
	}
	else if(Target_Speed_Cm_S<0)
	{
		Dc_Motor_Back();
		target_speed=-Target_Speed_Cm_S;
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);		//��ת���������ϼ���
	}
	
	while(1)
	{
		Led_Times(4,1);
		time_out++;
		
		if(Speed_Cm_S<Target_Speed_Cm_S-4||Speed_Cm_S>Target_Speed_Cm_S+4)Dc_Motor_SetDuty(Duty+(Target_Speed_Cm_S-Speed_Cm_S)/4);
		else{
			if(Speed_Cm_S<Target_Speed_Cm_S-1)Dc_Motor_SetDuty(Duty+1);
			else if(Speed_Cm_S>Target_Speed_Cm_S+1)Dc_Motor_SetDuty(Duty-1);
		}
		
		//�ж��Ƿ�ﵽ�趨�ٶ�
		if(Speed_Cm_S>target_speed-1&&Speed_Cm_S<target_speed+1)		
		{
			right_times++;
			if(right_times>=10)break;									//�ж��ٶ��Ƿ��ȶ�
		}
		else right_times=0;
		
		if(time_out>=100)
		{
			BlueTooth_TransmitPrintf("TIME OUT\r\n");
			Beep_Times(3,2);
			break;														//��ʱֱ���˳�����ֹ���ʲ��㿨��
		}
	}
	Beep_Times(3,1);
}

//С����ʻ�������룬֧�ָ����루���ˣ���GateΪ���Ǹ��ص�ռ�ձ���ֵ
//������ʻ�����PID����
float DC_Motor_Length_Kp=3.5;
float DC_Motor_Length_Ki=10;		//Iֵ���ֹ���ص�����ʻ����
float DC_Motor_Length_Kd=5;		//Dֵ���ֹ����
void Dc_Motor_Move(float Target_Length_Cm,int Gate)
{
	//����ǰ��Speed_Get()������0.1s��ʱ�ж��д򿪣�
	
	int target_length=Target_Length_Cm;
	int duty=0;
	float Sum_Length=0;
	//�趨��ʻ����
	if(Target_Length_Cm<0){
		Dc_Motor_Back();
		target_length=-Target_Length_Cm;
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);		//���÷�ת�������������ϼ���
	}
	else Dc_Motor_Go();
	
	//������ʻ���벢�����ٶ�
	Sum_Pulse=0;
	Sum_Length=0;
	while(Sum_Length < target_length-2)
	{
		Sum_Length=Sum_Pulse*PER_PULSE/10;
		duty=Pid_Control(DC_Motor_Length_Kp,DC_Motor_Length_Ki,DC_Motor_Length_Kd,target_length,Sum_Length, PID_INCREASE);
		Dc_Motor_SetDuty(duty+Gate);	//���Ǹ��أ��ƶ���ֵΪ20
	}
	
	//���ǹ��ԣ���ͣ
	Dc_Motor_Emerg_Stop();
}

//ֱ�������ʼ��
void Dc_Motor_Init(void)
{
	//ע�⣡������PB3/4/PA15��������Ĭ�ϵ���JLink�ĸ��ù��ܣ���������л�������GPIO�ڵ����ã������������ŵ�GPIO��ʼ��һ��Ҫ������������GPIO����֮�󣬷�����Ȼ�޷���Ϊ��ͨIOʹ�á�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	
	//Disable JLink, enable SW
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    

   	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	Dc_Motor_Stop();
}

//��ʾֱ�����Ŀǰ״����ռ�ձȣ�
void Show_DC_Motor_State()
{	
	//��ʾL298N��IN1234�ڵ�ƽ����������ã�
//	OLED_ShowString(1,1,"order:");
//	OLED_ShowNum(1,7,order,1);
//	OLED_ShowNum(2,1,GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13),1);
//	OLED_ShowNum(2,3,GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12),1);
//	OLED_ShowNum(3,1,GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9),1);
//	OLED_ShowNum(3,3,GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8),1);	
	Speed_Cm_S=Speed_Get(0.1);
	OLED_ShowString(1,1,"Speed:   cm/s");
	OLED_ShowNum(1,7,Speed_Cm_S,3);
	
	Duty=TIM_GetCapture1(TIM3);
	OLED_ShowString(2,1,"Duty:   %");
	OLED_ShowNum(2,7,Duty,3);
}

//�ⲿʱ�Ӽ�����������ģʽ����ֱ���������������ʼ��
void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;							//����GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					//�������� GPIO_Mode_IN_FLOATING�������루������������η�ֵ0.5v���ң�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//Ĭ���ڲ�ʱ�ӣ���ʡ��
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ʱ����Ԫ
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;				//ARR	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;				//PSC
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	//�������ӿ����ã�ͨ·��PPT��
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	

	TIM_Cmd(TIM4, ENABLE);
	
	Dc_Motor_SetDuty(0);
	
}

//��ȡ����������ֵ��ֱ�����ٵ����תת��������������
int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}

//����С���ٶȣ�cm/s��
float Speed_Get(int Time_S)
{
	int Pulse_Num=Encoder_Get();
	//��������������ֵ��ȥ��ǰֵ
	Sum_Pulse+=Pulse_Num;				//���������ۼӣ�Dc_Motor_Move()�õ���
	float Speed=Pulse_Num*0.134;		//ʵΪPulse_Num*PER_PULSE/Time_S/10��������д�����
	return Speed;
}

//��ң�ع���ʵ��
void Dc_Motor_Mode(){
	int duty;					//ռ�ձ��趨ֵ
	int angle=0;
	int Length=0;
	float speed=0;
	int order=0;				//�������յ�������
	int sign=1;					//ѭ����־λ
	
	while(sign){
		//���������źŴ�����
		if(Get_BlueTooth_ReceivePackFlag()==1)
		{
			BlueTooth_TransmitPrintf("%s\r\n",BlueTooth_RxPack);
			BlueTooth_TransmitPrintf("\r\nOK\r\n");
			order=BlueTooth_RxPack[0];
		}
		switch(order){
			case 0:Dc_Motor_Stop();break;
			case 1:Dc_Motor_Go();break;
			case 2:Dc_Motor_Back();break;
			case 3:duty=duty+10;Dc_Motor_SetDuty(duty);break;
			case 4:duty=duty-10;Dc_Motor_SetDuty(duty);break;
			case 5:Length+=5;Dc_Motor_Move(Length,0);break;
			case 6:Length-=5;Dc_Motor_Move(Length,0);break;
			case 7:angle+=10;Set_Angle(angle);break;
			case 8:angle-=10;Set_Angle(angle);break;
			case 9: speed+=10;Dc_Motor_SetSpeed_PID(speed);break;
			case 10:speed-=10;Dc_Motor_SetSpeed_PID(speed);break;
			case 11:Turn_Left_90();break;
			case 12:Turn_Right_90();break;
			case 13:Fast_Turn_Left_90();break;
			case 14:Fast_Turn_Right_90();break;
			case 15:sign=0;break;	//�˳�ѭ��
		}
		order=-1;
	}
}

void Simple_Aviod_Obstacle(){
	//�򵥱��Ϲ���ʵ��
	
	extern float distance;		//���������ֵ					//cs100a.c
	int Simple_Aviod_Obstacle_Stop_Sign=1;
	while(Simple_Aviod_Obstacle_Stop_Sign)
	{
		if(distance<=400){
			Dc_Motor_SetDuty(-100);Set_Angle(-60);
		}
		else if(distance>500&&distance<=1000){
			Dc_Motor_SetDuty(100);Set_Angle(60);
		}
		else{
			Dc_Motor_SetDuty(300);Set_Angle(0);
		}
	}
}
