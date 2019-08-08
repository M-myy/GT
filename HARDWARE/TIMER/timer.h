#ifndef _timer_h_
#define _timer_h_

#define PWM_ARR 899

void TIM3_pwm_init(unsigned int arr,unsigned int psc);  //pwm输出初始化
void TIM2_init(unsigned int arr,unsigned int psc);  //定时器初始化函数
void TIM5_Cap_Init(unsigned int arr,unsigned int psc);    //定时器5捕获模式初始化
void TIM4_init(unsigned int arr,unsigned int psc);  //定时器4初始化函数

#endif
