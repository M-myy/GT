#ifndef _timer_h_
#define _timer_h_

#define PWM_ARR 899

void TIM3_pwm_init(unsigned int arr,unsigned int psc);  //pwm�����ʼ��
void TIM2_init(unsigned int arr,unsigned int psc);  //��ʱ����ʼ������
void TIM5_Cap_Init(unsigned int arr,unsigned int psc);    //��ʱ��5����ģʽ��ʼ��
void TIM4_init(unsigned int arr,unsigned int psc);  //��ʱ��4��ʼ������

#endif
