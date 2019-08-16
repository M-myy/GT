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
#include "my_uart.h"

u8 key = 0;
extern float KP;
extern u16 ANGLE;
extern int PWM;
extern float SPEED;
extern u8 USART_RX_BUF[USART_REC_LEN];
u16 USART_TX_BUF[USART_REC_LEN] = {0};

int main()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	uart_init(115200);
	key_init();
	delay_init();
	TIM5_Cap_Init(0xffff,0);
	TIM2_init(4999,1399);
	TIM3_pwm_init(899,999);
	pid_init();
	TIM_SetCompare2(TIM3,899);  //开始时不让电机转动
	LCD_Init();
  AT24CXX_Init();			//IIC初始化
	TIM4_init(1249,349);

	LCD_Clear(WHITE);	
	zhujiemian();	
}
