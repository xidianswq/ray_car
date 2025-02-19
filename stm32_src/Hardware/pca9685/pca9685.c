#include "pca9685.h"

int Off_L=SERVO090;
int Off_V=SERVO090;

/*********************************************************
�������ܣ��ȴ�I2C�����¼�
*********************************************************/
void PCA9685_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}

/*********************************************************
�������ܣ���PCA9685��һ����ַд����
*********************************************************/
void PCA9685_WriteReg(uint8_t RegAddress, uint8_t Data)
{	
	I2C_GenerateSTART(I2C1, ENABLE);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, PCA9685_ADDRESS, I2C_Direction_Transmitter);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, RegAddress);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, Data);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/*********************************************************
�������ܣ���PCA9685��һ����ַ������
*********************************************************/
uint8_t PCA9685_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, PCA9685_ADDRESS, I2C_Direction_Transmitter);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, RegAddress);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, PCA9685_ADDRESS, I2C_Direction_Receiver);
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	PCA9685_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return Data;
}

/*********************************************************
�������ܣ�PCA9685Ƶ���޸�
��ڲ�����freq-���PWMƵ��
*********************************************************/
void PCA9685_SetPwmFreq(float freq) 
{
	int prescale,oldmode,newmode;
	float prescaleval;
	
	freq *= 1;   								//����Ƶ�������еĹ��壬����У׼								
	prescaleval = 25000000;							//���ݹ�ʽ����prescale��ֵ
	prescaleval /= 4096;							//prescaleval=round(osc_cloc/4096/freq)-1;
	prescaleval /= freq;
	prescaleval -= 1;
	prescale = floor(prescaleval + 0.5);		
                
	oldmode = PCA9685_ReadReg(PCA9685_MODE1);		//���MODE1�Ĵ���ֵ
	newmode = (oldmode&0x7F) | 0x10; 			  	//SLEEPλ ��1
	PCA9685_WriteReg(PCA9685_MODE1, newmode);  	  	//����SLEEPģʽ
	PCA9685_WriteReg(PCA9685_PRESCALE, prescale);    	//����Ƶ��
	PCA9685_WriteReg(PCA9685_MODE1, oldmode);		  	//�˳�SLEEPģʽ
	delay_us(5000);
	PCA9685_WriteReg(PCA9685_MODE1, oldmode | 0xa1);//RESTART��AI��ALLCALL����λ ��1
}

/*********************************************************
�������ܣ��ı�ͨ��PWMռ�ձ�
���������num-ʹ�õ�ͨ��0~15
		  on-�ߵ�ƽ��ʼʱ������ACKֵ
		  off-�ߵ�ƽ����ʱ������ACKֵ
*********************************************************/
void PCA9685_SetPwm(int num, int on, int off) 
{
	PCA9685_WriteReg(LED0_ON_L+4*num,on&0xff);		//LED0_ON_L����on��8λ
	PCA9685_WriteReg(LED0_ON_H+4*num,(on>>8)&0xff);	//LED0_ON_H����on��4λ
	PCA9685_WriteReg(LED0_OFF_L+4*num,off&0xff);
	PCA9685_WriteReg(LED0_OFF_H+4*num,(off>>8)&0xff);
 }

/*********************************************************
�������ܣ���ʼ��PCA9685
*********************************************************/
void PCA9685_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_Cmd(I2C1, ENABLE);
	
	PCA9685_WriteReg(PCA9685_MODE1,0x00);
	PCA9685_SetPwmFreq(50);
	int i=0;
	for(i=0;i<16;i++)
	{
		PCA9685_SetPwm(i,0,SERVO090);
	}
	Set_Angle_Vert(0);
}

/*********************************************************
�������ܣ���̨ˮƽ������ת
*********************************************************/
void Spinnig_Level(int diff)
{
	if(Off_L+diff<=SERVO000+SERVO_L_RANGE)
	{
		Off_L=SERVO000+SERVO_L_RANGE;
	}
	else if(Off_L+diff>=SERVO180-SERVO_L_RANGE)
	{
		Off_L=SERVO180-SERVO_L_RANGE;	
	}
	else
	{
		Off_L=Off_L+diff;
	}	
	PCA9685_SetPwm(0,0,Off_L);	
}

/*********************************************************
�������ܣ���̨��ֱ������ת
*********************************************************/
void Spinnig_Vert(int diff)
{
	if(diff<0)
	{
		Off_V=(Off_V+diff)<(SERVO000+SERVO_V_RANGE)?(SERVO000+SERVO_V_RANGE):(Off_V+diff);
	}
	else if(diff>0)
	{
		Off_V=(Off_V+diff)>(SERVO180-SERVO_V_RANGE)?(SERVO180-SERVO_V_RANGE):(Off_V+diff);	
	}
	PCA9685_SetPwm(1,0,Off_V);
}

/*********************************************************
�������ܣ���̨��ֱ�������ýǶ�
*********************************************************/
void Set_Angle_Vert(float angle)
{
	Off_V=angle*(SERVO180-SERVOSUB)/180+SERVOADJUST;	//ʹ�ý���ֵʹˮƽ
	if(Off_V<=SERVO000)Off_V=SERVO000;
	else if(Off_V>=SERVO180)Off_V=SERVO180;	
	PCA9685_SetPwm(1,0,Off_V);
}

/*********************************************************
�������ܣ���̨�������ĵ�
*********************************************************/
#include "usart.h"
#define LENGTH 1640	//����ϵX����
#define HEIGHT 1232	//����ϵY����
#define X_DIR 1	//���������ת����
#define Y_DIR -1	//���������ת����

int Follow_Point_Stop_Sign=1;
int x=LENGTH/2,y=HEIGHT/2;		//����㵱ǰ����
//��̨ˮƽ������תPIDֵ
float Level_Kp=0.03;
float Level_Ki=0.00;
float Level_Kd=0.00;
//��̨��ֱ������תPIDֵ
float Vert_Kp=0.03;
float Vert_Ki=0.00;
float Vert_Kd=0.00;

void Follow_Point(void)
{
	int Get_Point_Pos(void);

	int diff_x,diff_y;
	while(Follow_Point_Stop_Sign)
	{
		if(Get_Point_Pos()==2){
			diff_x=Pid_Control(Level_Kp,Level_Ki,Level_Kd,LENGTH/2,x,PID_INCREASE);
			diff_y=Pid_Control(Vert_Kp,Vert_Ki,Vert_Kd,HEIGHT/2,y,PID_INCREASE);
			Spinnig_Level(X_DIR*diff_x);
			Spinnig_Vert(Y_DIR*diff_y);
		}
		break;	//ʹִֻ��һ��
	}
}

int is_num(char c){
	if(c>='0'&&c<='9')return 1;
	else return 0;
}

extern char  USART_RX_INFO[USART_REC_LEN];	//uart��������
int Get_Point_Pos(void)
{
	int state=0,i,j;
	for(i=0;i<2;i++){
		for(j=0;j<4;j++){
			if(is_num(USART_RX_INFO[i*5+j+1]))continue;
			else return state;
		}
	}
	if(USART_RX_INFO[0]=='x')				//������ݶ�λ�Ƿ���ȷ(��λ��������ϢΪ��x1234y5678)
	{
		x=(USART_RX_INFO[1]-'0')*1000+(USART_RX_INFO[2]-'0')*100+(USART_RX_INFO[3]-'0')*10+USART_RX_INFO[4]-'0';
		if(x>0)state++;
	}
	if(USART_RX_INFO[5]=='y')				//������ݶ�λ�Ƿ���ȷ(��λ��������ϢΪ��x1234y5678)
	{
		y=(USART_RX_INFO[6]-'0')*1000+(USART_RX_INFO[7]-'0')*100+(USART_RX_INFO[8]-'0')*10+USART_RX_INFO[9]-'0';
		if(y>0)state++;
	}
	CLR_Buf();
	return state;							//���ؽ��ܳɹ���
}

/*********************************************************
�������ܣ���̨�������ֵ����Ϊnano����ֵ
*********************************************************/
int PWM_x=SERVO090,PWM_y=SERVO090;
void Nano_SetPWM(void){
	int Get_Nano_PWM(void);

	if(Get_Nano_PWM()==2){
		PCA9685_SetPwm(0,0,PWM_x);
		PCA9685_SetPwm(1,0,PWM_y);
	}
	
}

int Get_Nano_PWM(void)
{
	int state=0,i,j;
	for(i=0;i<2;i++){
		for(j=0;j<3;j++){
			if(is_num(USART_RX_INFO[i*4+j+1]))continue;
			else return state;
		}
	}
	if(USART_RX_INFO[0]=='x')				//������ݶ�λ�Ƿ���ȷ(��λ��������ϢΪ��x123y456)
	{
		x=(USART_RX_INFO[1]-'0')*100+(USART_RX_INFO[2]-'0')*10+(USART_RX_INFO[3]-'0');
		if(x>0)state++;
	}
	if(USART_RX_INFO[4]=='y')				//������ݶ�λ�Ƿ���ȷ(��λ��������ϢΪ��x123y456)
	{
		y=(USART_RX_INFO[5]-'0')*100+(USART_RX_INFO[6]-'0')*10+(USART_RX_INFO[7]-'0');
		if(y>0)state++;
	}
	CLR_Buf();
	return state;							//���ؽ��ܳɹ���
}

/*********************************************************
�������ܣ���̨��JOYSTICK����������
*********************************************************/
#include "AD.h"
extern float Voltage[2];
#define X_MAX_VOLTAGE 330	//x���ѹֵ����
#define Y_MAX_VOLTAGE 320	//y���ѹֵ����
#define V_X_DIR -1	//���������ת����
#define V_Y_DIR 1	//���������ת����

int v_x=X_MAX_VOLTAGE/2,v_y=Y_MAX_VOLTAGE/2;		//����㵱ǰ����
//��̨ˮƽ������תPIDֵ
float v_x_Kp=0.01;
float v_x_Ki=0;
float v_x_Kd=0.003;
//��̨��ֱ������תPIDֵ
float v_y_Kp=0.01;
float v_y_Ki=0;
float v_y_Kd=0.003;

void JoyStick_Control(void)
{
	int JoyStick_Control_Stop_Sign=1;
	int diff_x,diff_y;
	while(JoyStick_Control_Stop_Sign)
	{
		v_x=100*Voltage[0];v_y=100*Voltage[1];
		diff_x=Pid_Control(v_x_Kp,v_x_Ki,v_x_Kd,X_MAX_VOLTAGE/2,v_x,PID_INCREASE);
		diff_y=Pid_Control(v_y_Kp,v_y_Ki,v_y_Kd,Y_MAX_VOLTAGE/2,v_y,PID_INCREASE);
		Spinnig_Level(V_X_DIR*diff_x);
		Spinnig_Vert(V_Y_DIR*diff_y);
	}
}
