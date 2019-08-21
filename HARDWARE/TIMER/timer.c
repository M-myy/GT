#include "timer.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "key.h"
#include "lcd.h"
#include "my_printf.h"

int cnt = 0;
unsigned int time = 0;
unsigned char TIM5CH_Cap_Sta = 0;
float REAL_SPEED;  //���ת��
extern u8 key;

void TIM2_init(unsigned int arr,unsigned int psc)	          //��ʱ����ʼ������
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);       //ʹ�ܶ�ʱ����ʱ��
	
	TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;      //����Ƶ
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                   //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStru.TIM_Period=arr;                      //��������
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStru);             //��ʼ����ʱ��2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                  //ʹ�ܶ�ʱ��2����ʱ���ж�
	 
	NVIC_InitStru.NVIC_IRQChannel=TIM2_IRQn;                  //�жϷ�������
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;                  //���ܷ��ж�ͨ��
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=0;        //�ж����ȼ�Ϊ0
	NVIC_InitStru.NVIC_IRQChannelSubPriority=0;               //�ж������ȼ�(��Ӧ���ȼ�)Ϊ0
	NVIC_Init(&NVIC_InitStru);
	
	TIM_Cmd(TIM2,ENABLE);                                     //ʹ�ܶ�ʱ���ж�2
}

void TIM2_IRQHandler(void)                                  //��ʱ��2�жϷ�����
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update))                  //���Ϊ����ж�
	{
		cnt = TIM5->CNT;                                        //��ȡ��ʱ����ļ���ֵ
		REAL_SPEED = (float)(short)cnt/4/26;
		my_printf("speed: %fc/s\r\n",REAL_SPEED);                  //��ӡת�٣�260Ϊ���դ�񣬹�ʽΪ"Tʱ���ڼ���ֵ/4(һ��դ������Ĵ�����)/դ����"
		TIM_SetCounter(TIM5,0);                                 //��ն�ʱ��5�ļ�����
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);             //����жϱ�־
	}
}

void TIM3_pwm_init(unsigned int arr,unsigned int psc)       //��ʱ��3ͨ��1���pwm����
{
	GPIO_InitTypeDef GPIO_InitStru;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	TIM_OCInitTypeDef TIM_OCInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);                       //ʹ�ܶ�ʱ��3��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);  //ʹ��ioʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);                       //��ӳ��
	
	GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;                                  //��������ģʽ
	GPIO_InitStru.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStru);
	
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;                  //���������ϼ���ģʽ
	TIM_TimeBaseInitStru.TIM_Period=arr;                                      //��ʱ������
	TIM_TimeBaseInitStru.TIM_ClockDivision = 0;                               //����Ƶ
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                                   //����Ԥ��Ƶϵ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStru);                             //��ʼ����ʱ��3��������
	
	TIM_OCInitStru.TIM_OCMode=TIM_OCMode_PWM2;                                //pwmģʽ
	TIM_OCInitStru.TIM_OCPolarity=TIM_OCPolarity_High;                        //�ߵ�ƽ
	TIM_OCInitStru.TIM_OutputState=TIM_OutputState_Enable;                    //ʹ�����
	TIM_OC2Init(TIM3,&TIM_OCInitStru);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);            //ʹ�ܸ����ж�
	
	NVIC_InitStru.NVIC_IRQChannel=TIM3_IRQn;            //�жϷ���ͨ��
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;            //ʹ���ж�
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=1;  //�ж����ȼ�Ϊ1
	NVIC_InitStru.NVIC_IRQChannelSubPriority=1;         //�����ȼ�(��Ӧ���ȼ�)Ϊ1
	NVIC_Init(&NVIC_InitStru);
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	TIM_Cmd(TIM3,ENABLE);  //ʹ�ܶ�ʱ��3
}

void TIM3_IRQHandler(void)  //��ʱ��3�жϷ�����
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))  //�ж��ж�
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //����жϱ�־λ
	}
}

void TIM5_Cap_Init(unsigned int arr,unsigned int psc)  //��ʱ��5��ʼ������
{
	GPIO_InitTypeDef GPIO_InitStru;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	TIM_ICInitTypeDef TIM_ICInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��gpioaʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);    //ʹ�ܶ�ʱ��5ʱ��
	
	GPIO_InitStru.GPIO_Mode=GPIO_Mode_IN_FLOATING;    //��������ģʽ
	GPIO_InitStru.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;     //ѡ������0��1
	GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;        //50MHZ����
	GPIO_Init(GPIOA,&GPIO_InitStru);                  //��ʼ������������
	
	TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;    //��Ƶ����1������Ƶ��
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ�����ģʽ
	TIM_TimeBaseInitStru.TIM_Period=arr;                    //����
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                 //Ԥ��Ƶϵ��
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStru);           //��ʼ����ʱ����������
	
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);  //��׽��ʱ��ͨ��1��2����Ƶϵ��Ϊ4
    TIM_ICStructInit(&TIM_ICInitStru);
	
	TIM_ICInitStru.TIM_ICFilter=0x03;    //�˲����8��
	TIM_ICInit(TIM5,&TIM_ICInitStru);
	
	NVIC_InitStru.NVIC_IRQChannel=TIM5_IRQn;               //��ʱ��5�ж�����   
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;               //ʹ���ж�
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=0;     //�ж����ȼ�Ϊ0
	NVIC_InitStru.NVIC_IRQChannelSubPriority=1;            //�����ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStru);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);     //����жϱ�־λ
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  //���״̬��־λ
	TIM5->CNT = 0;                            //�����ʱ��5�����Ĵ���
	
	TIM_Cmd(TIM5,ENABLE);
}

void TIM5_IRQHandler(void)     //�����ϵ������ʱ���жϺ��������ò�������Ϊ0.1��Ķ�ʱ��2�Ȳ�����ʱ��5����ͻ����TIM5->CNT��Ϊ�����������
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update))
	{
	  TIM_ClearITPendingBit(TIM5,TIM_IT_Update);    //����жϱ�־
	}
}

void TIM4_init(unsigned int arr,unsigned int psc)	//��ʱ����ʼ������
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;        //����Ƶ
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;    //���ϼ���ģʽ
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                     //����Ԥ��Ƶϵ��
	TIM_TimeBaseInitStru.TIM_Period=arr;                        //���ü�������
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStru);               //��ʼ������
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                    //ʹ�ܶ�ʱ��4�����ж�
	
	NVIC_InitStru.NVIC_IRQChannel=TIM4_IRQn;             //ʹ�ܶ�ʱ��4�ж�        
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;             //ʹ���ж�
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=1;   //�ж����ȼ�Ϊ1 
	NVIC_InitStru.NVIC_IRQChannelSubPriority=2;          //�ж������ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStru);                           //��ʼ���ж����� 
	
	TIM_Cmd(TIM4,DISABLE);  //�ȹر��Ա���Ӱ����ʾ
}

void TIM4_IRQHandler(void)
{
    static int i = 1;
	static float LAST_SPEED = 0.0;                 //�洢��һ���ٶ�ֵ
	
	if(TIM_GetITStatus(TIM4, TIM_IT_Update))       //�ж��Ƿ�Ϊ�����ж�
	{
	  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//����жϱ�־Ϊ
	  LCD_Fill(20+i,200,20+i,300,WHITE);
		LCD_DrawLine(20+i,(u16)(300-LAST_SPEED*10),21+i,(u16)(300-REAL_SPEED*10));
		LAST_SPEED = REAL_SPEED;    //������һ�ε�ֵ
		i++;
    if(i>=210)i = 1;
	}
}
