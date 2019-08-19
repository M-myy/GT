#include "pid.h"
#include "stdlib.h"
#include "usart.h"	

float KP = 0.0;  //pid系数
float KI = 0.0;
float KD = 0.0;
extern float REAL_SPEED;

struct PID  //pid参数结构体
{
	float SetSpeed;
	float ActualSpeed;
	float err;
	float err_last;
	float Kp,Ki,Kd;
	float voltage;
	float integral;
  int pwm;
}pid;

void pid_init(void)  //pid参数初始化
{
	pid.ActualSpeed = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.integral = 0.0;
}

int pid_realize(float speed)  //pid实现函数，返回值为pwm值
{	
	pid.Kp = KP;
	pid.Ki = KI;
	pid.Kd = KD;
	pid.SetSpeed=speed;
	pid.ActualSpeed = REAL_SPEED;
  pid.err=pid.SetSpeed-pid.ActualSpeed;
	pid.integral+=pid.err;
  pid.voltage=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
	pid.err_last = pid.err;
  pid.pwm=pid.voltage;
	if(pid.pwm<0)
		pid.pwm = -pid.pwm;
  return pid.pwm;
}
