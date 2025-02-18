#include "ray.h"
#define VAILD 0
int L_2=0;
int L_1=0;
int C=0;
int R_1=0;
int R_2=0;
int State=0;	//С������״̬��0Ϊֹͣ��1Ϊֱ��

//��ʼ����ȡ���⴫������GPIO
void Ray_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//��ȡ�������⴫������ֵ
void Get_Ray_State(void)
{
	L_2=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
	L_1=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
	C=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	R_1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	R_2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
}

//���⴫��״̬��ģ�棬�ⲿ״̬�仯����Stop_Sign���˳�ѭ��
void Ray_State_Action(void)
{
	int Stop_Sign=1;//���ӹ���ֹͣλ
	while(Stop_Sign)
	{
		Get_Ray_State();
		//ȫδ��⵽ֱ��
		if(L_2==L_1&&L_1==C&&C==R_1&&R_1==R_2&&R_2==!VAILD)
		{
			;
		}
		//ֻ���м�ļ�⵽ֱ��
		else if(L_2==L_1&&L_1==R_1&&R_1==R_2&&R_2==!VAILD&&C==VAILD)
		{
			;
		}
		//ֻ��һ��⵽ֱ��
		else if(L_2==C&&C==R_1&&R_1==R_2&&R_2==!VAILD&&L_1==VAILD)
		{
			;
		}
		//ֻ�����⵽ֱ��
		else if(L_1==C&&C==R_1&&R_1==R_2&&R_2==!VAILD&&L_2==VAILD)
		{
			;
		}
		//ֻ��һ��⵽ֱ��
		else if(R_2==C&&C==L_1&&L_1==L_2&&L_2==!VAILD&&R_1==VAILD)
		{
			;
		}
		//ֻ�Ҷ���⵽ֱ��
		else if(R_1==C&&C==L_1&&L_1==L_2&&L_2==!VAILD&&R_2==VAILD)
		{
			;
			
		}
		//ȫ��⵽ֱ��
		else if(L_2==L_1&&L_1==C&&C==R_1&&R_1==R_2&&R_2==VAILD)
		{
			;
		}
		//�������
		else
		{
			;
		}
	}
}

//Ѳ�߳���Duty����[0,500]
void Follow_Track(int duty)
{
	int Follow_Track_Stop_Sign=1;
	while(Follow_Track_Stop_Sign)
	{
		Get_Ray_State();
		//ȫδ��⵽ֱ��
		if(L_2==L_1&&L_1==C&&C==R_1&&R_1==R_2&&R_2==!VAILD)
		{
//			Dc_Motor_Stop();
		}
		//ֻ���м�ļ�⵽ֱ��
		else if(L_2==L_1&&L_1==R_1&&R_1==R_2&&R_2==!VAILD&&C==VAILD)
		{
			//ֱ��
			Set_Angle(0);	
			Dc_Motor_Go();
			Dc_Motor_SetDuty(duty);
		}
		//ֻ��һ��⵽ֱ��
		else if(L_2==C&&C==R_1&&R_1==R_2&&R_2==!VAILD&&L_1==VAILD)
		{
			//��΢��ת
			Set_Angle(-30);
			Dc_Motor_Go();
			Dc_Motor_SetDuty(duty);
		}
		//ֻ�����⵽ֱ��
		else if(L_1==C&&C==R_1&&R_1==R_2&&R_2==!VAILD&&L_2==VAILD)
		{
			//��ת
			Set_Angle(-60);
			Dc_Motor_Go();
			Dc_Motor_SetDuty(duty);
		}
		//ֻ��һ��⵽ֱ��
		else if(R_2==C&&C==L_1&&L_1==L_2&&L_2==!VAILD&&R_1==VAILD)
		{
			//��΢��ת
			Set_Angle(30);
			Dc_Motor_Go();
			Dc_Motor_SetDuty(duty);
		}
		//ֻ�Ҷ���⵽ֱ��
		else if(R_1==C&&C==L_1&&L_1==L_2&&L_2==!VAILD&&R_2==VAILD)
		{
			//��ת
			Set_Angle(60);
			Dc_Motor_Go();
			Dc_Motor_SetDuty(duty);
		}
		//ȫ��⵽ֱ��
		else if(L_2==L_1&&L_1==C&&C==R_1&&R_1==R_2&&R_2==VAILD)
		{
//			Dc_Motor_Stop();
		}
		//�������
		else
		{
			;
		}
	}
}
