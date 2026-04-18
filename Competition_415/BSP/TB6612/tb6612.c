#include "tb6612.h"
#include "questions.h"
#include "timer.h"

int speed_L = 0;
int speed_R = 0;
int motor_last_time = 0;
char buffer[256];

/******************************************************************
 * 函 数 名 称：TB6612_Motor_Stop
 * 函 数 说 明：A端和B端电机停止
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void TB6612_Motor_Stop(void)
{
    AIN1_OUT(1);
    AIN2_OUT(1);
    BIN1_OUT(1);
    BIN2_OUT(1);
}

/******************************************************************
 * 函 数 名 称：AO_Control
 * 函 数 说 明：A端口电机控制
 * 函 数 形 参：dir旋转方向 1正转0反转   speed旋转速度，范围（0 ~ per-1）
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：speed 0-1000
******************************************************************/
void L_Control(char dir, int speed)
{
    if(speed > 999)
    {
        speed = 999;
    }

    if( dir == 1 )
    {
        AIN1_OUT(0);
        AIN2_OUT(1);
    }
    else
    {
        AIN1_OUT(1);
        AIN2_OUT(0);
    }

    speed_L = speed;
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, speed, GPIO_PWM_MOTOR_C0_IDX);
}



/******************************************************************
 * 函 数 名 称：BO_Control
 * 函 数 说 明：B端口电机控制
 * 函 数 形 参：dir旋转方向 1正转0反转   speed旋转速度，范围（0 ~ per-1）
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：speed 0-1000
******************************************************************/
void R_Control(char dir, int speed)
{
    if(speed > 999)
    {
        speed = 999;
    }

    if( dir == 1 )
    {
        BIN1_OUT(1);
        BIN2_OUT(0);
    }
    else
    {
        BIN1_OUT(0);
        BIN2_OUT(1);
    }

    speed_R = speed;
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, speed, GPIO_PWM_MOTOR_C1_IDX);
}

// 控制左右轮转速，相当于原来的 Motion_Car_Control
void TB_Control(int L_speed, int R_speed)
{
    if(L_speed >= 0) L_Control(1,L_speed);
    else L_Control(0, -L_speed);
    if(R_speed >= 0) R_Control(1,R_speed);
    else R_Control(0, -R_speed);

    // sprintf(buffer,"v_L = %d, v_R = %d\r\n", L_speed, R_speed);
    // USART_SendString(buffer);

}

void TB_Get_Odometry(void)// 用 速度*时间 来计算里程数
{
    // if(encoder_odometry_flag)
    // {   
    //     int speed = (speed_L + speed_R) / 2;
    //     odometry_sum += speed * (Get_Time() - motor_last_time) / 1000.0;
    // }
    // motor_last_time = Get_Time();
    // sprintf(buffer,"s = %f\r\n", odometry_sum);
    // USART_SendString(buffer);
}

void Go_Straight()
{
    TB_Control(500, 500);
}
void Turn_Left()
{
    TB_Control(-250, 750);
}