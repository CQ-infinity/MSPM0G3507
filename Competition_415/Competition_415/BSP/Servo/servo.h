#ifndef __BSP_SERVO_H_
#define __BSP_SERVO_H_

#include "ti_msp_dl_config.h"


extern float Y_DIS;
extern float now_x, now_z;

void Set_Servo270_Angle(float angle);
void Set_Servo_Angle(float angle);
unsigned int Get_Servo_Angle(void);
unsigned int Get_Servo_Angle270(void);

void Set_TH(float angle);
void Set_PH(float angle);
unsigned int Get_TH(void);
unsigned int Get_PH(void);



void Set_PWM(int pwma,int pwmb);
int angle_to_pwm_270(float angle);
int angle_to_pwm_180(float angle);
float base_x(float y,float x);
float base_z(float y,float z);
void move_Point(float x, float y, float z);


#endif

