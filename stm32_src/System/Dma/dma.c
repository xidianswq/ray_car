#include "dma.h"

void BlueToothDmaInit(uint8_t *DMA_MemoryBaseAddr)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef BlueToothDmaInitStructure;
	BlueToothDmaInitStructure.DMA_PeripheralBaseAddr=(uint32_t)&USART2->DR;			//����վ����ʼ��ַ
	BlueToothDmaInitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;	//���ݿ��
	BlueToothDmaInitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;			//�Ƿ�����
	BlueToothDmaInitStructure.DMA_MemoryBaseAddr=(uint32_t)DMA_MemoryBaseAddr;		//�洢��վ����ʼ��ַ
	BlueToothDmaInitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;		//���ݿ��
	BlueToothDmaInitStructure.DMA_MemoryInc=DMA_PeripheralInc_Enable;				//�Ƿ�����
	BlueToothDmaInitStructure.DMA_Mode=DMA_Mode_Circular;							//�Ƿ�ʹ���Զ���װ
	BlueToothDmaInitStructure.DMA_BufferSize=1;										//��������С�������������
	BlueToothDmaInitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;						//���䷽��
	BlueToothDmaInitStructure.DMA_M2M=DMA_MemoryInc_Disable;						//Ӳ�������������
	BlueToothDmaInitStructure.DMA_Priority=DMA_Priority_High;						//���ȼ�
	
	DMA_Init(DMA1_Channel6,&BlueToothDmaInitStructure);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	
	DMA_Cmd(DMA1_Channel6,ENABLE);
}

void Uart1DmaInit(uint8_t *DMA_MemoryBaseAddr,uint32_t DMA_BufferSize)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef UartDmaInitStructure;
	UartDmaInitStructure.DMA_PeripheralBaseAddr=(uint32_t)&USART2->DR;			//����վ����ʼ��ַ
	UartDmaInitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;	//���ݿ��
	UartDmaInitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;			//�Ƿ�����
	UartDmaInitStructure.DMA_MemoryBaseAddr=(uint32_t)DMA_MemoryBaseAddr;		//�洢��վ����ʼ��ַ
	UartDmaInitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;		//���ݿ��
	UartDmaInitStructure.DMA_MemoryInc=DMA_PeripheralInc_Enable;				//�Ƿ�����
	UartDmaInitStructure.DMA_Mode=DMA_Mode_Circular;							//�Ƿ�ʹ���Զ���װ
	UartDmaInitStructure.DMA_BufferSize=DMA_BufferSize;							//��������С�������������
	UartDmaInitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;							//���䷽��
	UartDmaInitStructure.DMA_M2M=DMA_MemoryInc_Disable;							//Ӳ�������������
	UartDmaInitStructure.DMA_Priority=DMA_Priority_High;						//���ȼ�
	
	DMA_Init(DMA1_Channel5,&UartDmaInitStructure);
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	DMA_Cmd(DMA1_Channel5,ENABLE);
}
