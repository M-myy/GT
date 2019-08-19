#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "stdlib.h"
#include "lcd.h"

/*********************************************************************************************
*���ⲿ����Ϊ�������а�����ʹ����PG2-8���߸�IO�����ڰ���ɨ�裬����PG2-4����ɨ�裬PG5-8������ *
*ɨ�衣
**********************************************************************************************/

extern u8 key;

void key_init(void)    //���󰴼���ʼ������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);    //ʹ��GPIOG���ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;     //ѡ��2��3��4��Ϊ��ɨ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //����Ϊ�������룬�����м��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;    //ѡ����ĸ�������ɨ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //����Ϊ���������������ɨ��
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

unsigned char key_scan(void)    //����ɨ�躯��
{
	static unsigned char HANG1_UP = 1;    //��¼��һ�ε�һ�а���״ֵ̬
	static unsigned char HANG2_UP = 1;    //��¼��һ�εڶ��а���״ֵ̬
	static unsigned char HANG3_UP = 1;    //��¼��һ�ε����а���״ֵ̬
	static unsigned char HANG4_UP = 1;    //��¼��һ�ε����а���״ֵ̬
	
	HANG1 = 0;    //���͵�һ������
	if(HANG1_UP&&((LIE1 == 0)||(LIE2 == 0)||(LIE3 == 0)))    //����ж���
	{
		delay_ms(10);    //��ʱ10��������
		HANG1_UP = 0;    //����״ֵ̬����ֹ����
		if(LIE1 == 0){HANG1 = 1;return KEY1_PRESS;}  //��һ���ж���
    if(LIE2 == 0){HANG1 = 1;return KEY2_PRESS;}  //�ڶ����ж���
		if(LIE3 == 0){HANG1 = 1;return KEY3_PRESS;}  //�������ж���
	}
	else if((LIE1 == 1)&&(LIE2 == 1)&&(LIE3 == 1))  //���Ϊ��������û�ж���
		HANG1_UP = 1;    //��״ֵ̬������Ϊ1
	HANG1 = 1;  //�������ߵ�һ��
	
	HANG2 = 0;  //���͵ڶ�������
	if(HANG2_UP&&((LIE1 == 0)||(LIE2 == 0)||(LIE3 == 0)))    //����ж���
	{
		delay_ms(10);
		HANG2_UP = 0;
		if(LIE1 == 0){HANG2 = 1;return KEY4_PRESS;}  //��һ���ж���
    if(LIE2 == 0){HANG2 = 1;return KEY5_PRESS;}  //�ڶ����ж���
		if(LIE3 == 0){HANG2 = 1;return KEY6_PRESS;}  //�������ж���
	}
	else if((LIE1 == 1)&&(LIE2 == 1)&&(LIE3 == 1))  //���Ϊ��������û�ж���
		HANG2_UP = 1;    //��״ֵ̬������Ϊ1
	HANG2 = 1;  //�������ߵڶ���
	
	HANG3 = 0;
	if(HANG3_UP&&((LIE1 == 0)||(LIE2 == 0)||(LIE3 == 0)))
	{
		delay_ms(10);
		HANG3_UP = 0;
		if(LIE1 == 0){HANG3 = 1;return KEY7_PRESS;}
    if(LIE2 == 0){HANG3 = 1;return KEY8_PRESS;}
		if(LIE3 == 0){HANG3 = 1;return KEY9_PRESS;}  
	}
	else if((LIE1 == 1)&&(LIE2 == 1)&&(LIE3 == 1))
		HANG3_UP = 1;
	HANG3 = 1;
	
	HANG4 = 0;
	if(HANG4_UP&&((LIE1 == 0)||(LIE2 == 0)||(LIE3 == 0)))
	{
		delay_ms(10);
		HANG4_UP = 0;
		if(LIE1 == 0){HANG4 = 1;return KEY10_PRESS;}
    if(LIE2 == 0){HANG4 = 1;return KEY11_PRESS;}
		if(LIE3 == 0){HANG4 = 1;return KEY12_PRESS;}
	}
	else if((LIE1 == 1)&&(LIE2 == 1)&&(LIE3 == 1))
		HANG4_UP = 1;
	HANG4 = 1;
	
	return 0;
}
