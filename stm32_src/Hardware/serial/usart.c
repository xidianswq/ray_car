#include "usart.h"	
#include "pca9685.h"
//��-A9-TX ��-A10-RX
u16 point1 = 0;
   	
extern int x,y;							//�Ӳ�����	//PCA9685.c	
	
char USART_RX_INFO[USART_REC_LEN];	  	//�������ݻ��棨��������ʶͷβ��
const char head_tag = '#';
const char tail_tag = '$';
uint8_t RxData;
uint16_t RxPackFlag;
  
void uart_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

	CLR_Buf();//��ջ���
}

void USART1_IRQHandler(void)                				//����1�жϷ������
{
	static uint16_t RxPackState=0;
	static uint16_t RxPackPoint=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) 
	{
		RxData =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if(RxPackState == 0)
		{
			static uint16_t Cmd_Point=0;
			if(RxData == head_tag && Cmd_Point == 0)
			{	
				Cmd_Point=1;
				RxPackState=1;
				RxPackPoint=0;
			}	
			else Cmd_Point=0;
		}
		else if(RxPackState == 1)
		{
			if (RxData == tail_tag)
			{
				RxPackState = 0;
				RxPackFlag = 1;
				USART_RX_INFO[RxPackPoint]='\0';
			}
			else USART_RX_INFO[RxPackPoint++]=RxData;
		}
	}
}

void CLR_Buf(void)                           				// ���ڻ�������
{
	memset(USART_RX_INFO, 0, USART_REC_LEN);      			//���
  point1 = 0;                    
}
