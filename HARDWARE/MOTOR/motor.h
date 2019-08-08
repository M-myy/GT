#ifndef _MOTOR_H_
#define _MOTOR_H_

void motor_start(float speed);
void motor_angle(unsigned int angle);
void pid_set(void);
void change(void);
void angle_set(void);

#endif
