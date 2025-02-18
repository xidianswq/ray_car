#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"
#include "string.h"

#define USART_REC_LEN 64  					//�����������ֽ��� 64

extern const char head_tag;					//��־ͷ
extern const char tail_tag;					//��־β
extern uint8_t RxData;
extern uint16_t RxPackFlag;
extern char USART_RX_INFO[USART_REC_LEN];	//�������ݻ��棨��������ʶͷβ��

extern int p_x,p_y,p_len;							//����������ǰ���꼰����

void uart_init(u32 bound);
void CLR_Buf(void);

#endif
