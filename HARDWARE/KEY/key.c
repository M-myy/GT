#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "stdlib.h"
#include "lcd.h"

/*********************************************************************************************
*该外部按键为四行三列按键，使用了PG2-8共七个IO口用于按键扫描，其中PG2-4用列扫描，PG5-8用于行 *
*扫描。
**********************************************************************************************/

extern u8 key;

void key_init(void)    //矩阵按键初始化函数
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);    //使能GPIOG组的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;     //选择2，3，4作为列扫描
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //设置为上拉输入，用于列检测
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;    //选择该四个用于行扫描
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //设置为推挽输出，用于行扫描
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

unsigned char key_scan(void)    //按键扫描函数
{
	static unsigned char HANG1_UP = 1;    //记录上一次第一行按键状态值
	static unsigned char HANG2_UP = 1;    //记录上一次第二行按键状态值
	static unsigned char HANG3_UP = 1;    //记录上一次第三行按键状态值
	static unsigned char HANG4_UP = 1;    //记录上一次第四行按键状态值
	
	HANG1 = 0;    //拉低第一行引脚
	if(HANG1_UP&&((LIE1 == 0)||(LIE2 == 0)||(LIE3 == 0)))    //如果有动作
	{
		delay_ms(10);    //延时10毫秒消抖
		HANG1_UP = 0;    //更新状态值，防止连按
		if(LIE1 == 0){HANG1 = 1;return KEY1_PRESS;}  //第一列有动作
    if(LIE2 == 0){HANG1 = 1;return KEY2_PRESS;}  //第二列有动作
		if(LIE3 == 0){HANG1 = 1;return KEY3_PRESS;}  //第三列有动作
	}
	else if((LIE1 == 1)&&(LIE2 == 1)&&(LIE3 == 1))  //如果为抖动，即没有动作
		HANG1_UP = 1;    //将状态值继续设为1
	HANG1 = 1;  //重新拉高第一行
	
	HANG2 = 0;  //拉低第二行引脚
	if(HANG2_UP&&((LIE1 == 0)||(LIE2 == 0)||(LIE3 == 0)))    //如果有动作
	{
		delay_ms(10);
		HANG2_UP = 0;
		if(LIE1 == 0){HANG2 = 1;return KEY4_PRESS;}  //第一列有动作
    if(LIE2 == 0){HANG2 = 1;return KEY5_PRESS;}  //第二列有动作
		if(LIE3 == 0){HANG2 = 1;return KEY6_PRESS;}  //第三列有动作
	}
	else if((LIE1 == 1)&&(LIE2 == 1)&&(LIE3 == 1))  //如果为抖动，即没有动作
		HANG2_UP = 1;    //将状态值继续设为1
	HANG2 = 1;  //重新拉高第二行
	
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
