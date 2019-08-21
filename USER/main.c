#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "timer.h"
#include "motor.h"
#include "pid.h"
#include "eeprom.h"
#include "lcd.h"
#include "jiemian.h"
#include "iic.h"
#include "my_printf.h"

/*****************************************************************
*  key:���ڶ�ȡ������ֵ                                           *
*  KP:�ⲿ�����������������ϵ��KP                                 *
*  ANGLE:�ⲿ�����������������ת���Ƕ�                            *
*  PWM:�ⲿ�����������������pwm                                   *
*  SPEED:�ⲿ�����������������ת��                                *
*  USART_RX_BUF[USART_REC_LEN]:�ⲿ�������飬���ڽ��մ�������      *
******************************************************************/

u8 key = 0;
extern float KP;
extern u16 ANGLE;
extern int PWM;
extern float SPEED;
extern u8 USART_RX_BUF[USART_REC_LEN];

int main()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ�������
	uart_init(115200);                              //���ô��ڲ�����
	key_init();                                     //����ɨ���ʼ��
	delay_init();                                   //��ʱ������ʼ��
	TIM5_Cap_Init(0xffff,0);                        //��ʱ��5����Ϊ��������ģʽ��ʼ��
	TIM2_init(4999,1399);                           //��ʱ��2��ʼ�������ڶ�ȡ��ʱ��5���������ֵ����ʱΪ0.1��
	TIM3_pwm_init(899,999);                         //��ʱ���������ڲ���pwm
	pid_init();                                     //pid������ʼ��
	TIM_SetCompare2(TIM3,899);                      //��ʼʱ���õ��ת��
	LCD_Init();                                     //��ʼ��LCD
        AT24CXX_Init();			                //EEPROM��ʼ��
	TIM4_init(2499,1399);                            //��ʱ��4��ʼ����������ʾ���δ��

	LCD_Clear(WHITE);	                        //����
	zhujiemian();	                                //����������
}

