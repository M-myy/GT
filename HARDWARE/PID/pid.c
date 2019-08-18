#include "pid.h"
#include "stdlib.h"
#include "usart.h"	

float KP = 0.0;
float KI = 0.0;
float KD = 0.0;
extern float REAL_SPEED;

struct PID
{
	float SetSpeed;
	float ActualSpeed;
	float err;
	float err_last;
	float err_next;
	float Kp,Ki,Kd;
	float voltage;
  int pwm;
}pid;

void pid_init(void)
{
	pid.ActualSpeed = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
}

int pid_realize(float speed)
{	
	float incrementSpeed;
	pid.Kp = KP;
	pid.Ki = KI;
	pid.Kd = KD;
	pid.SetSpeed=speed;
	pid.err=pid.SetSpeed-pid.ActualSpeed;
	incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
	pid.ActualSpeed = REAL_SPEED;
	pid.voltage+=incrementSpeed;
	pid.err_last=pid.err_next;
  pid.err_next=pid.err;
  pid.pwm=pid.voltage;
	if(pid.pwm<0)
		pid.pwm = -pid.pwm;
  return pid.pwm;
}
