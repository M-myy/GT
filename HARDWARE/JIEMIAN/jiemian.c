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
		TIM_SetCompare2(TIM3,899);
		key = key_scan();   //扫描按键值
		if(key==KEY1_PRESS)
		{
		    pid_jiemian();
			TIM_Cmd(TIM4,ENABLE);  //一定不能提前开启，经测试提前开启会影响其他的显示异常
			while(1)
			{
				key = key_scan();
				pid_set();
			}
		}
		if(key==KEY2_PRESS)
		{
			angle_jiemian();
			while(1)
			{
				key = key_scan();
			    angle_set();
			}
		}
  }
}

void pid_jiemian(void)
{
	AT24CXX_Read(10,P,20);
	AT24CXX_Read(40,I,20);
	AT24CXX_Read(70,D,20);
	AT24CXX_Read(100,V,20);
	
	KP = atof((char *)P);
	KI = atof((char *)I);
	KD = atof((char *)D);
	SPEED = atof((char *)V);
	
	LCD_Clear(WHITE);
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
}

void angle_jiemian(void)
{
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,90,24,24,"1.ANGLE:");
	LCD_ShowxNum(120,40,ANGLE,3,24,0);
	LCD_ShowString(30,70,100,24,24,"2.Return");
}

void boxing_jiemian(void)
{
	if(SPEED!=LAST_SPEED)
	  LCD_Fill(10,200,230,300,WHITE);
	LCD_DrawLine(10,300-(int)(SPEED*10),230,300-(int)(SPEED*10));
	LCD_Fast_DrawPoint(10+i,300-(int)(REAL_SPEED*10),GREEN);
	LCD_Fill(10+i,200,10+i,300,WHITE);
	i++;
	if(i>=220)i = 0;
}

