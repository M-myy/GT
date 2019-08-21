#include "jiemian.h"
#include "lcd.h"
#include "motor.h"
#include "key.h"
#include "eeprom.h"
#include "stdlib.h"

extern u16 ANGLE;
extern u8 key;
extern u8 P[20],I[20],D[20],V[20];
extern int PWM;
extern float SPEED,KP,KI,KD;
extern float REAL_SPEED;
float LAST_REAL_SPEED = 0.0,LAST_SPEED = 0.0;
u8 i = 1;

void zhujiemian(void)
{
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,50,24,24,"1.PID");
	LCD_ShowString(30,70,90,24,24,"2.Angle");
	
	while(1)
	{
		TIM_SetCompare2(TIM3,899);  //未收到指令前电机禁止转动
		key = key_scan();    //扫描按键值
		if(key==KEY1_PRESS)  //进入pid调速模式
		{
		    pid_jiemian();       //显示pid界面
			TIM_Cmd(TIM4,ENABLE);  //一定不能提前开启，经测试提前开启会影响其他的显示异常
			while(1)
			{
				key = key_scan();    //按键扫描
				pid_set();           //pid设置
			}
		}
		if(key==KEY2_PRESS)      //进入转动角度模式
		{
			angle_jiemian();       //角度界面
			while(1)
			{
				key = key_scan();    //按键扫描
			    angle_set();       //角度设置
			}
		}
  }
}

void pid_jiemian(void)
{
	AT24CXX_Read(10,P,20);    //进入pid界面时读取一遍eeprom保存的值
	AT24CXX_Read(40,I,20);
	AT24CXX_Read(70,D,20);
	AT24CXX_Read(100,V,20);
	
	KP = atof((char *)P);    //转换读到的值由字符型转换为浮点型
	KI = atof((char *)I);
	KD = atof((char *)D);
	SPEED = atof((char *)V);
	
	LCD_Clear(WHITE);    //以下为pid界面的显示
	LCD_ShowString(30,40,50,24,24,"1.P:");
	LCD_ShowString(80,40,120,24,24,P);
	LCD_ShowString(30,70,50,24,24,"2.I:");
	LCD_ShowString(80,70,100,24,24,I);
	LCD_ShowString(30,100,50,24,24,"3.D:");
	LCD_ShowString(80,100,100,24,24,D);
	LCD_ShowString(30,130,100,24,24,"4.SPEED:");
	LCD_ShowString(130,130,100,24,24,V);
	LCD_ShowString(30,160,100,24,24,"5.Return");
	LCD_Set_Window(10,200,220,100);  //开窗显示波形
	LCD_Fill(10,199,230,199,RED);
	LCD_Fill(20,199,20,300,GREEN);
	LCD_Fill(10,300-SPEED*10,20,300-SPEED*10,RED);   //显示预定速度
}

void angle_jiemian(void)
{
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,90,24,24,"1.ANGLE:");
	LCD_ShowxNum(120,40,ANGLE,3,24,0);
	LCD_ShowString(30,70,100,24,24,"2.Return");
}


