#include "motor.h"
#include "pid.h"
#include "timer.h"
#include "stm32f10x.h"
#include "key.h"
#include "lcd.h"
#include "stdio.h"
#include "delay.h"
#include "jiemian.h"
#include "eeprom.h"

u8 P[20] = "0",I[20] = "0",D[20] = "0",V[20] = "0";
int PWM = 0;
u16 ANGLE = 0;
float SPEED = 0.0;
extern float KP;
extern float KI;
extern float KD;
extern u8 key;
extern u8 USART_RX_BUF[200];
extern u16 USART_RX_STA;

void motor_start(float speed)
{
	PWM = pid_realize(speed);
	if(PWM>=PWM_ARR)
	PWM = PWM_ARR;  //控制PWM值得范围
	TIM_SetCompare2(TIM3,PWM);
}

void motor_angle(unsigned int angle)
{
	u16 maichongshu;
	maichongshu = angle*14.444;  //一度大约对应14.444个脉冲
	TIM_SetCompare2(TIM3,899);  //确保不转
	while(TIM5->CNT!=0)
	  TIM5->CNT = 0;  //防止波动，清零定时器5计数器
  while(TIM5->CNT<maichongshu)
	{
		key = key_scan();
		TIM_SetCompare2(TIM3,835);  //转动开始
		if(key==KEY12_PRESS)
			break;
	}	
//	delay_ms(10000);
  TIM_SetCompare2(TIM3,899);	
	TIM5->CNT = 0;  //再次清0计数器
}

void change(void)
{
	sprintf((char *)P,"%f",KP);
	sprintf((char *)I,"%f",KI);
	sprintf((char *)D,"%f",KD);
  sprintf((char *)V,"%f",SPEED);
}

void pid_set(void)
{	
	switch(key)
	{
		case KEY1_PRESS:
		{
			LCD_Fill(5,40,25,64,GREEN);
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){KP = KP+1.0;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY2_PRESS){KP = KP+0.1;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY3_PRESS){KP = KP+0.01;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY4_PRESS){KP = KP+0.001;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY5_PRESS){KP = KP+0.0001;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY6_PRESS){KP = KP+0.00001;change();LCD_ShowString(80,40,120,24,24,P);}
				
				else if(key==KEY7_PRESS){KP = KP-1.0;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY8_PRESS){KP = KP-0.1;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY9_PRESS){KP = KP-0.01;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY10_PRESS){KP = KP-0.001;change();LCD_ShowString(80,40,120,24,24,P);}
				else if(key==KEY11_PRESS){KP = KP-0.0001;change();LCD_ShowString(80,40,120,24,24,P);}
				
				else if(key==KEY12_PRESS){eeprom_write_dat(P,10,20);break;}
			}
			LCD_Fill(5,40,25,64,WHITE);break;
		}
		case KEY2_PRESS:
		{
			LCD_Fill(5,70,25,94,GREEN);
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){KI = KI+0.01;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY2_PRESS){KI = KI+0.001;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY3_PRESS){KI = KI+0.0001;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY4_PRESS){KI = KI+0.00001;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY5_PRESS){KI = KI+0.000001;change();LCD_ShowString(80,70,100,24,24,I);}
				
				else if(key==KEY7_PRESS){KI = KI-0.01;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY8_PRESS){KI = KI-0.001;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY9_PRESS){KI = KI-0.0001;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY10_PRESS){KI = KI-0.00001;change();LCD_ShowString(80,70,100,24,24,I);}
				else if(key==KEY11_PRESS){KI = KI-0.000001;change();LCD_ShowString(80,70,100,24,24,I);}
				
				else if(key==KEY12_PRESS)break;
			}
			LCD_Fill(5,70,25,94,WHITE);break;
		}
		case KEY3_PRESS:
		{
			LCD_Fill(5,100,25,124,GREEN);
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){KD = KD+0.1;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY2_PRESS){KD = KD+0.01;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY3_PRESS){KD = KD+0.001;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY4_PRESS){KD = KD+0.0001;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY5_PRESS){KD = KD+0.00001;change();LCD_ShowString(80,100,100,24,24,D);}
				
				else if(key==KEY7_PRESS){KD = KD-0.1;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY8_PRESS){KD = KD-0.01;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY9_PRESS){KD = KD-0.001;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY10_PRESS){KD = KD-0.0001;change();LCD_ShowString(80,100,100,24,24,D);}
				else if(key==KEY11_PRESS){KD = KD-0.00001;change();LCD_ShowString(80,100,100,24,24,D);}
				
				else if(key==KEY12_PRESS)break;
			}
			LCD_Fill(5,100,25,124,WHITE);break;
	  }
		case KEY4_PRESS:
		{
			LCD_Fill(5,130,25,154,GREEN);
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){SPEED = SPEED+1.0;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY2_PRESS){SPEED = SPEED+0.1;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY3_PRESS){SPEED = SPEED+0.01;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY4_PRESS){SPEED = SPEED+0.001;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY5_PRESS){SPEED = SPEED+0.0001;change();LCD_ShowString(130,130,100,24,24,V);}
				
				else if(key==KEY7_PRESS){SPEED = SPEED-1.0;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY8_PRESS){SPEED = SPEED-0.1;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY9_PRESS){SPEED = SPEED-0.01;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY10_PRESS){SPEED = SPEED-0.001;change();LCD_ShowString(130,130,100,24,24,V);}
				else if(key==KEY11_PRESS){SPEED = SPEED-0.0001;change();LCD_ShowString(130,130,100,24,24,V);}
				
				else if(key==KEY12_PRESS)break;
			}
			LCD_Fill(5,130,25,154,WHITE);
			motor_start(SPEED);break;
		}
		case KEY12_PRESS:
		{
			TIM_Cmd(TIM4,DISABLE);  //退出首先关闭定时器4，不再显示波形而且不影响主界面显示
			LCD_Fill(10,200,230,300,WHITE);  //清屏波形区域
			zhujiemian();
			break;
		}
	}
	eeprom_write_dat(P,10,20);
	eeprom_write_dat(I,40,20);
	eeprom_write_dat(D,70,20);
	eeprom_write_dat(V,100,20);
	
	if(USART_RX_STA&0x8000)   //串口控制PID设置
	{
		if(USART_RX_BUF[0]=='P'){KP=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(80,40,120,24,24,P);}
		else if(USART_RX_BUF[0]=='I'){KI=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(80,70,100,24,24,I);}
		else if(USART_RX_BUF[0]=='D'){KD=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(80,100,100,24,24,D);}
		else if(USART_RX_BUF[0]=='V'){SPEED=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(130,130,100,24,24,V);}
		USART_RX_STA=0;
	}
	motor_start(SPEED);
}

void angle_set(void)
{		
	switch(key)
	{
		case KEY1_PRESS:
		{
			LCD_Fill(5,40,25,64,GREEN);
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){ANGLE = ANGLE+100;LCD_ShowxNum(120,40,ANGLE,3,24,0);}
				else if(key==KEY2_PRESS){ANGLE = ANGLE+10;LCD_ShowxNum(120,40,ANGLE,3,24,0);}
				else if(key==KEY3_PRESS){ANGLE = ANGLE+1;LCD_ShowxNum(120,40,ANGLE,3,24,0);}
				
				else if(key==KEY7_PRESS){ANGLE = ANGLE-100;LCD_ShowxNum(120,40,ANGLE,3,24,0);}
				else if(key==KEY8_PRESS){ANGLE = ANGLE-10;LCD_ShowxNum(120,40,ANGLE,3,24,0);}
				else if(key==KEY9_PRESS){ANGLE = ANGLE-1;LCD_ShowxNum(120,40,ANGLE,3,24,0);}
				
				else if(key==KEY12_PRESS){break;}
			}
			LCD_Fill(5,40,25,64,WHITE);
			motor_angle(ANGLE);
			break;
		}
		case KEY12_PRESS:
		{
			zhujiemian();break;
		}
	}
}
