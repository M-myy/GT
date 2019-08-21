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
*  key:用于读取按键的值                                           *
*  KP:外部定义变量，用于设置系数KP                                 *
*  ANGLE:外部定义变量，用于设置转动角度                            *
*  PWM:外部定义变量，用于设置pwm                                   *
*  SPEED:外部定义变量，用于设置转速                                *
*  USART_RX_BUF[USART_REC_LEN]:外部定义数组，用于接收串口数据      *
******************************************************************/

u8 key = 0;
extern float KP;
extern u16 ANGLE;
extern int PWM;
extern float SPEED;
extern u8 USART_RX_BUF[USART_REC_LEN];

int main()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组设置
	uart_init(115200);                              //设置串口波特率
	key_init();                                     //按键扫描初始化
	delay_init();                                   //延时函数初始化
	TIM5_Cap_Init(0xffff,0);                        //定时器5设置为正交编码模式初始化
	TIM2_init(4999,1399);                           //定时器2初始化，用于读取定时器5的脉冲计数值，定时为0.1秒
	TIM3_pwm_init(899,999);                         //定时器三，用于产生pwm
	pid_init();                                     //pid参数初始化
	TIM_SetCompare2(TIM3,899);                      //开始时不让电机转动
	LCD_Init();                                     //初始化LCD
        AT24CXX_Init();			                //EEPROM初始化
	TIM4_init(2499,1399);                            //定时器4初始化，用于显示波形打点

	LCD_Clear(WHITE);	                        //清屏
	zhujiemian();	                                //进入主界面
}

