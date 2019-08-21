#include "timer.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "key.h"
#include "lcd.h"
#include "my_printf.h"

int cnt = 0;
unsigned int time = 0;
unsigned char TIM5CH_Cap_Sta = 0;
float REAL_SPEED;  //电机转速
extern u8 key;

void TIM2_init(unsigned int arr,unsigned int psc)	          //定时器初始化函数
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);       //使能定时器二时钟
	
	TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;      //不分频
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                   //预分频系数
	TIM_TimeBaseInitStru.TIM_Period=arr;                      //计数周期
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStru);             //初始化定时器2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                  //使能定时器2更新时间中断
	 
	NVIC_InitStru.NVIC_IRQChannel=TIM2_IRQn;                  //中断分组配置
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;                  //是能否中断通道
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=0;        //中断优先级为0
	NVIC_InitStru.NVIC_IRQChannelSubPriority=0;               //中断子优先级(响应优先级)为0
	NVIC_Init(&NVIC_InitStru);
	
	TIM_Cmd(TIM2,ENABLE);                                     //使能定时器中断2
}

void TIM2_IRQHandler(void)                                  //定时器2中断服务函数
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update))                  //如果为溢出中断
	{
		cnt = TIM5->CNT;                                        //读取定时器五的计数值
		REAL_SPEED = (float)(short)cnt/4/26;
		my_printf("speed: %fc/s\r\n",REAL_SPEED);                  //打印转速，260为电机栅格，公式为"T时间内计数值/4(一个栅格会检测四次脉冲)/栅格数"
		TIM_SetCounter(TIM5,0);                                 //清空定时器5的计数器
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);             //清除中断标志
	}
}

void TIM3_pwm_init(unsigned int arr,unsigned int psc)       //定时器3通道1输出pwm函数
{
	GPIO_InitTypeDef GPIO_InitStru;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	TIM_OCInitTypeDef TIM_OCInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);                       //使能定时器3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);  //使能io时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);                       //重映射
	
	GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;                                  //复用推挽模式
	GPIO_InitStru.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStru);
	
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;                  //计数器向上计数模式
	TIM_TimeBaseInitStru.TIM_Period=arr;                                      //定时器周期
	TIM_TimeBaseInitStru.TIM_ClockDivision = 0;                               //不分频
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                                   //设置预分频系数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStru);                             //初始化定时器3基础设置
	
	TIM_OCInitStru.TIM_OCMode=TIM_OCMode_PWM2;                                //pwm模式
	TIM_OCInitStru.TIM_OCPolarity=TIM_OCPolarity_High;                        //高电平
	TIM_OCInitStru.TIM_OutputState=TIM_OutputState_Enable;                    //使能输出
	TIM_OC2Init(TIM3,&TIM_OCInitStru);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);            //使能更新中断
	
	NVIC_InitStru.NVIC_IRQChannel=TIM3_IRQn;            //中断分组通道
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;            //使能中断
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=1;  //中断优先级为1
	NVIC_InitStru.NVIC_IRQChannelSubPriority=1;         //子优先级(响应优先级)为1
	NVIC_Init(&NVIC_InitStru);
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	TIM_Cmd(TIM3,ENABLE);  //使能定时器3
}

void TIM3_IRQHandler(void)  //定时器3中断服务函数
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))  //判断中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
	}
}

void TIM5_Cap_Init(unsigned int arr,unsigned int psc)  //定时器5初始化函数
{
	GPIO_InitTypeDef GPIO_InitStru;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	TIM_ICInitTypeDef TIM_ICInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能gpioa时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);    //使能定时器5时钟
	
	GPIO_InitStru.GPIO_Mode=GPIO_Mode_IN_FLOATING;    //浮空输入模式
	GPIO_InitStru.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;     //选择引脚0和1
	GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;        //50MHZ速率
	GPIO_Init(GPIOA,&GPIO_InitStru);                  //初始化所配置引脚
	
	TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;    //分频设置1（不分频）
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;//向上计数，模式
	TIM_TimeBaseInitStru.TIM_Period=arr;                    //周期
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                 //预分频系数
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStru);           //初始化定时器基础设置
	
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);  //捕捉定时器通道1，2，倍频系数为4
    TIM_ICStructInit(&TIM_ICInitStru);
	
	TIM_ICInitStru.TIM_ICFilter=0x03;    //滤波检测8次
	TIM_ICInit(TIM5,&TIM_ICInitStru);
	
	NVIC_InitStru.NVIC_IRQChannel=TIM5_IRQn;               //定时器5中断配置   
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;               //使能中断
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=0;     //中断优先级为0
	NVIC_InitStru.NVIC_IRQChannelSubPriority=1;            //子优先级为1
	NVIC_Init(&NVIC_InitStru);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);     //清除中断标志位
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  //清除状态标志位
	TIM5->CNT = 0;                            //清除定时器5计数寄存器
	
	TIM_Cmd(TIM5,ENABLE);
}

void TIM5_IRQHandler(void)     //理论上电机测速时该中断函数根本用不到，因为0.1秒的定时器2等不到定时器5溢出就会清除TIM5->CNT，为保险起见加上
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update))
	{
	  TIM_ClearITPendingBit(TIM5,TIM_IT_Update);    //清除中断标志
	}
}

void TIM4_init(unsigned int arr,unsigned int psc)	//定时器初始化函数
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	NVIC_InitTypeDef NVIC_InitStru;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;        //不分频
	TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;    //向上计数模式
	TIM_TimeBaseInitStru.TIM_Prescaler=psc;                     //设置预分频系数
	TIM_TimeBaseInitStru.TIM_Period=arr;                        //设置计数周期
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStru);               //初始化配置
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                    //使能定时器4更新中断
	
	NVIC_InitStru.NVIC_IRQChannel=TIM4_IRQn;             //使能定时器4中断        
	NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;             //使能中断
	NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=1;   //中断优先级为1 
	NVIC_InitStru.NVIC_IRQChannelSubPriority=2;          //中断子优先级为2
	NVIC_Init(&NVIC_InitStru);                           //初始化中断配置 
	
	TIM_Cmd(TIM4,DISABLE);  //先关闭以避免影响显示
}

void TIM4_IRQHandler(void)
{
    static int i = 1;
	static float LAST_SPEED = 0.0;                 //存储上一次速度值
	
	if(TIM_GetITStatus(TIM4, TIM_IT_Update))       //判断是否为更新中断
	{
	  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//清除中断标志为
	  LCD_Fill(20+i,200,20+i,300,WHITE);
		LCD_DrawLine(20+i,(u16)(300-LAST_SPEED*10),21+i,(u16)(300-REAL_SPEED*10));
		LAST_SPEED = REAL_SPEED;    //更新上一次的值
		i++;
    if(i>=210)i = 1;
	}
}
