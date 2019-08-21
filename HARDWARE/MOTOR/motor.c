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
#include "my_printf.h"

u8 P[20] = "0",I[20] = "0",D[20] = "0",V[20] = "0";  //LCD显示使用
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
	maichongshu = angle*2.98;  //一度大约对应的脉冲
	TIM_SetCompare2(TIM3,899);  //确保不转
	while(TIM5->CNT!=0)
	  TIM5->CNT = 0;  //防止波动，清零定时器5计数器
	TIM_Cmd(TIM2,DISABLE);   //关闭定时器2，防止清0定时器5的计数器
    while(TIM5->CNT<maichongshu)
	{
		key = key_scan();
		TIM_SetCompare2(TIM3,850);  //转动开始
		if(key==KEY12_PRESS)   //随时跳出
			break;
	}
	TIM_Cmd(TIM2,ENABLE);  //重新打开定时器2
  TIM_SetCompare2(TIM3,899);	
	TIM5->CNT = 0;  //再次清0计数器
}

void change(void)  //转换函数，将浮点数转换保存到字符数组中
{
	my_sprintf((char *)P,"%f",KP);
	my_sprintf((char *)I,"%f",KI);
	my_sprintf((char *)D,"%f",KD);
  my_sprintf((char *)V,"%f",SPEED);
}

/**************************************************************
*由于实际测试如果一直向eeprom中写入数据会影响界面显示以及按键扫描 *
*所以只在有数据更新后才保存数据，以减小影响                      *
***************************************************************/
void pid_set(void)
{	
	switch(key)
	{
		case KEY1_PRESS:
		{
			LCD_Fill(5,40,25,64,GREEN);  //指示方块（以下相同不在注释）
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){KP = KP+1.0;change();LCD_ShowString(80,40,96,24,24,P);}
				else if(key==KEY2_PRESS){KP = KP+0.1;change();LCD_ShowString(80,40,96,24,24,P);}
				else if(key==KEY3_PRESS){KP = KP+0.01;change();LCD_ShowString(80,40,96,24,24,P);}
				else if(key==KEY4_PRESS){KP = KP+0.001;change();LCD_ShowString(80,40,96,24,24,P);}
				
				else if(key==KEY7_PRESS){KP = KP-1.0;change();LCD_ShowString(80,40,96,24,24,P);}
				else if(key==KEY8_PRESS){KP = KP-0.1;change();LCD_ShowString(80,40,96,24,24,P);}
				else if(key==KEY9_PRESS){KP = KP-0.01;change();LCD_ShowString(80,40,96,24,24,P);}
				else if(key==KEY10_PRESS){KP = KP-0.001;change();LCD_ShowString(80,40,96,24,24,P);}
				
				else if(key==KEY12_PRESS){break;}  //按键12起返回作用
			}
			AT24CXX_Write(10,P,20);             //向24C02中写入要保存的数值
			LCD_Fill(5,40,25,64,WHITE);break;   //清除指示方块
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
			AT24CXX_Write(40,I,20);
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
			AT24CXX_Write(70,D,20);
			LCD_Fill(5,100,25,124,WHITE);break;
	  }
		case KEY4_PRESS:
		{
			LCD_Fill(5,130,25,154,GREEN);
			LCD_Fill(10,200,19,300,WHITE);
			while(1)
			{
				key = key_scan();
				if(key==KEY1_PRESS){SPEED = SPEED+1.0;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY2_PRESS){SPEED = SPEED+0.1;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY3_PRESS){SPEED = SPEED+0.01;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY4_PRESS){SPEED = SPEED+0.001;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY5_PRESS){SPEED = SPEED+0.0001;change();LCD_ShowString(130,130,72,24,24,V);}
				
				else if(key==KEY7_PRESS){SPEED = SPEED-1.0;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY8_PRESS){SPEED = SPEED-0.1;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY9_PRESS){SPEED = SPEED-0.01;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY10_PRESS){SPEED = SPEED-0.001;change();LCD_ShowString(130,130,72,24,24,V);}
				else if(key==KEY11_PRESS){SPEED = SPEED-0.0001;change();LCD_ShowString(130,130,72,24,24,V);}
				
				else if(key==KEY12_PRESS)break;
			}
			LCD_Fill(20,199,20,300,GREEN);
			LCD_Fill(10,300-SPEED*10,20,300-SPEED*10,RED);   //显示预定速度
			LCD_Fill(5,130,25,154,WHITE);
			AT24CXX_Write(100,V,20);
			motor_start(SPEED);break;
		}
		case KEY12_PRESS:
		{
			TIM_Cmd(TIM4,DISABLE);  //退出首先关闭定时器4，不再显示波形而且不影响主界面显示
			LCD_Fill(10,200,230,300,WHITE);  //清屏波形区域
			zhujiemian();  //进入主界面
			break;         //实际执行不到这一步
		}
	}
	
	if(USART_RX_STA&0x8000)   //串口控制PID设置
	{
		if(USART_RX_BUF[0]=='P'){KP=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(80,40,120,24,24,P);AT24CXX_Write(10,P,20);}
		else if(USART_RX_BUF[0]=='I'){KI=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(80,70,100,24,24,I);AT24CXX_Write(40,I,20);}
		else if(USART_RX_BUF[0]=='D'){KD=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(80,100,100,24,24,D);AT24CXX_Write(70,D,20);}
		else if(USART_RX_BUF[0]=='V'){LCD_Fill(10,200,19,300,WHITE);SPEED=atof((char *)(USART_RX_BUF+1));change();LCD_ShowString(130,130,100,24,24,V);LCD_Fill(20,199,20,300,GREEN);LCD_Fill(10,300-SPEED*10,20,300-SPEED*10,RED);AT24CXX_Write(100,V,20);}
		USART_RX_STA=0;  //清零状态位
	}
	motor_start(SPEED);
}

void angle_set(void)  //角度设置
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
	
	if(USART_RX_STA&0x8000)   //串口控制角度设置
	{
		if(USART_RX_BUF[0]=='A'){ANGLE = atof((char *)(USART_RX_BUF+1));LCD_ShowxNum(120,40,ANGLE,3,24,0);motor_angle(ANGLE);}
		USART_RX_STA=0;  //清零状态位
	}
}
