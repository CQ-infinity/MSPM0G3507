#ifndef	__QUESTIONS_H__
#define __QUESTIONS_H__

#include "ti_msp_dl_config.h"
#include "app_motor.h"
#include "app_irtracking.h"
#include "led.h"
#include "servo.h"
#include "tb6612.h"
#include "usart.h"
#include "servo_usart.h"

extern float odometry_sum;
extern int encoder_odometry_flag;
extern float Servo_error;
extern float PID_Value;
extern bool  adjustFirstAngle;
extern float first_angle;

extern int q1_circle_N;
extern int q1_circle_num;
extern int q1_turn_num;

extern int turn_flag;

//question state definition
#define  STOP_STATE   0
#define  RUN_STATE    1
#define  QUESTION_1   1
#define  QUESTION_2   2
#define  QUESTION_3   3
#define  QUESTION_4   4
#define  QUESTION_5   5
#define  QUESTION_6   6

//State_Machine
struct state_machine
{
    int Main_State;
	int Q1_State;
	int Q2_State;
	int Q3_State;
	int Q4_State;
	int Q5_State;
	int Q6_State;
	
};

extern volatile struct state_machine State_Machine;
void State_Machine_init(void);
void Question_Task_1(void);
void Question_Task_2(void);
void Question_Task_3(void);
void Question_Task_4(void);
void Question_Task_5(void);
void Question_Task_6(void);


#endif
