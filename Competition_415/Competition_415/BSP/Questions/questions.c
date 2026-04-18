//小车的question是需要做到循迹，与云台无关

#include"timer.h"
#include "questions.h"  // 任务题目头文件（包含状态定义、函数声明及全局变量类型）

// 全局变量：用于小车运动控制与状态判断
float object_angle = 0;                    // 小车运动的目标航向角（控制转向的基准角度）
float Angle_error = 0;                     // 角度误差（当前航向角与目标角的差值，用于PID调节）
int current_angle = 0;                     // 当前航向角（实时反馈的姿态角，来自陀螺仪或编码器）
float odometry_sum = 0;                    // 里程计累计值（记录小车行驶的总距离，单位通常为cm）
float first_angle = 0;                     // 初始角度（用于校准航向角基准，如启动时的初始方向）
int encoder_odometry_flag = 0;             // 编码器里程有效标志（1：允许累计里程；0：禁止累计，用于控制里程计数开关）
float PID_Value = 0;                       // PID控制输出值（用于速度或角度闭环调节的计算结果）
bool  adjustFirstAngle = 0;                // 初始角度校准标志（1：需要校准初始角度；0：已校准）

// 二级任务状态定义（每个任务的子状态，用于拆分复杂任务为多步骤执行）
#define  Q2_STATE_1   1  // 任务2子状态1（如"搜索目标"）
#define  Q2_STATE_2   2  // 任务2子状态2（如"瞄准目标"）
#define  Q2_STATE_3   3  // 任务2子状态3（如"等待确认"）
#define  Q2_STATE_4   4  // 任务2子状态4（如"执行动作"）
#define  Q2_STATE_5   5  // 任务2子状态5（如"任务结束"）
#define  Q3_STATE_1   1  // 任务3子状态1
#define  Q3_STATE_2   2  // 任务3子状态2
#define  Q3_STATE_3   3  // 任务3子状态3
#define  Q3_STATE_4   4  // 任务3子状态4
#define  Q3_STATE_5   5  // 任务3子状态5

// 状态机结构体实例（volatile修饰：确保在中断/多任务环境下变量值实时更新，避免编译器优化导致的逻辑错误）
volatile struct state_machine State_Machine;

// 任务首次执行标志（用于任务初始化，确保初始化操作仅执行一次）
int q1_first_flag = 0;  // 任务1首次执行标志（0：未执行初始化；1：已执行）
int q2_first_flag = 0;  // 任务2首次执行标志
int q3_first_flag = 0;  // 任务3首次执行标志
int q4_first_flag = 0;  // 任务4首次执行标志
int q5_first_flag = 0;  // 任务5首次执行标志
int q6_first_flag = 0;  // 任务6首次执行标志

// 循迹任务计数变量（用于记录完成的圈数和转弯次数）
int q1_circle_N = 0;    // 任务1目标圈数（需要完成的总圈数）
int q4_circle_N = 1;    // 任务4目标圈数（1圈）
int q5_circle_N = 2;    // 任务5目标圈数（2圈）
int q6_circle_N = 1;    // 任务6目标圈数（1圈）
int q1_circle_num = 0;  // 任务1当前完成圈数
int q4_circle_num = 0;  // 任务4当前完成圈数
int q5_circle_num = 0;  // 任务5当前完成圈数
int q6_circle_num = 0;  // 任务6当前完成圈数
int q1_turn_num = 0;    // 任务1当前转弯次数（每完成4次转弯记为1圈）
int q4_turn_num = 0;    // 任务4当前转弯次数
int q5_turn_num = 0;    // 任务5当前转弯次数
int q6_turn_num = 0;    // 任务6当前转弯次数

int turn_flag = 0;


int LineCheckTimes = 0; // 循迹检测次数（用于判断是否持续在轨迹上，避免误判）
char buffer[80];        // 字符串缓冲区（用于调试信息格式化输出）


/**
 * @brief 状态机初始化函数（系统启动时调用，将所有状态重置为停止状态）
 * 功能：确保系统上电后处于已知的初始状态，避免未定义行为（如随机状态导致的异常动作）
 */
void State_Machine_init(void)
{
    State_Machine.Main_State = STOP_STATE;       // 主状态：停止（系统级状态，控制所有任务的总开关）
    State_Machine.Q1_State = STOP_STATE;         // 任务1子状态：停止（任务级状态，控制单个任务的执行）
    State_Machine.Q2_State = STOP_STATE;         // 任务2子状态：停止
    State_Machine.Q3_State = STOP_STATE;         // 任务3子状态：停止
    State_Machine.Q4_State = STOP_STATE;         // 任务4子状态：停止
    State_Machine.Q5_State = STOP_STATE;         // 任务5子状态：停止
    State_Machine.Q6_State = STOP_STATE;         // 任务6子状态：停止
}


/**
 * @brief 任务1执行函数（循迹任务：按轨迹行驶指定圈数后停止）
 * 流程：初始化→循迹行驶→累计转弯次数→判断是否完成目标圈数→任务结束
 */

int t0 = 0;

void Question_Task_1(void)
{


    // 若任务1子状态为停止状态，直接返回（不执行任何操作）
    if(State_Machine.Q1_State == STOP_STATE) return;

    // 首次进入任务1：执行初始化操作（仅执行一次）
    if(q1_first_flag == 0)
    {
        encoder_odometry_flag = 1;  // 允许里程计数（开启编码器里程累计）
        q1_first_flag = 1;          // 标记为已初始化，避免重复执行
    }


    // 核心逻辑：执行循迹控制（调用LineWalking()函数，通过红外传感器数据控制小车沿轨迹行驶）
    // TB_Control(-250, 750);return;
    // sprintf(buffer,"q1, %d\r\n", Get_Time());
    // USART_SendString(buffer);
    if(turn_flag)
    {
        TB_Control(-250, 750);
        u8 x1,x2,x3,x4,x5,x6,x7,x8;
        deal_IRdata(&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8);
        if(x1 == 1 && x2 == 1 && x4 == 0 && x5 == 0)
        {
            turn_flag = 0;
            q1_turn_num ++;
        }
    }
    else LineWalking();

    // 转弯次数计数：每完成4次转弯，视为完成1圈
    if(q1_turn_num >= 4)
    {
        q1_circle_num ++;  // 完成圈数+1
        q1_turn_num = 0;  // 重置转弯次数，开始统计下一圈
    }

    // 结束条件：当前完成圈数≥目标圈数（q1_circle_N）
    if(q1_circle_num >= q1_circle_N)
    {
        bee_time = 400;    // 蜂鸣器响400ms（提示任务完成）
        // 更新状态机：主状态和任务1子状态均设为停止
        State_Machine.Main_State = STOP_STATE;
        State_Machine.Q1_State = STOP_STATE;

        q1_first_flag = 0; // 重置首次执行标志（为下次任务做准备）
        encoder_odometry_flag = 0; // 禁止里程计数（停止累计里程）
        odometry_sum = 0;  // 重置里程计（清零累计距离）
    }
}


/**
 * @brief 任务4执行函数（循迹任务：与任务1逻辑一致，目标圈数为1圈）
 */
void Question_Task_4(void)
{
    // 首次进入任务4：初始化（允许里程计数，标记已初始化）
    if(q4_first_flag == 0)
    {
        encoder_odometry_flag = 1;
        q4_first_flag = 1;
    }

    // 执行循迹控制
    if(turn_flag)
    {
        TB_Control(-250, 750);
        u8 x1,x2,x3,x4,x5,x6,x7,x8;
        deal_IRdata(&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8);
        if(x1 == 1 && x2 == 1 && x4 == 0 && x5 == 0)
        {
            turn_flag = 0;
            q4_turn_num ++;
        }
    }
    else LineWalking();

    // 转弯次数≥4时，完成1圈，重置转弯计数
    if(q4_turn_num >= 4)
    {
        q4_circle_num++;
        q4_turn_num = 0;
    }

    // 完成1圈后结束任务
    if(q4_circle_num >= q4_circle_N)
    {
        bee_time = 400;    // 蜂鸣器提示
        State_Machine.Main_State = STOP_STATE;
        State_Machine.Q4_State = STOP_STATE;

        q4_first_flag = 0;
        encoder_odometry_flag = 0;
        odometry_sum = 0;
    }
}


/**
 * @brief 任务5执行函数（循迹任务：目标圈数为2圈）
 */
void Question_Task_5(void)
{
    // 首次进入任务5：初始化
    if(q5_first_flag == 0)
    {
        encoder_odometry_flag = 1;
        q5_first_flag = 1;
    }

    // 执行循迹控制
    if(turn_flag)
    {
        TB_Control(-250, 750);
        u8 x1,x2,x3,x4,x5,x6,x7,x8;
        deal_IRdata(&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8);
        if(x1 == 1 && x2 == 1 && x4 == 0 && x5 == 0)
        {
            turn_flag = 0;
            q5_turn_num ++;
        }
    }
    else LineWalking();

    // 转弯次数≥4时，完成1圈
    if(q5_turn_num >= 4)
    {
        q5_circle_num++;
        q5_turn_num = 0;
    }

    // 完成2圈后结束任务
    if(q5_circle_num >= q5_circle_N)
    {
        bee_time = 400;
        State_Machine.Main_State = STOP_STATE;
        State_Machine.Q5_State = STOP_STATE;

        q5_first_flag = 0;
        encoder_odometry_flag = 0;
        odometry_sum = 0;
    }
}


/**
 * @brief 任务6执行函数（循迹任务：目标圈数为1圈）
 */
void Question_Task_6(void)
{
    // 首次进入任务6：初始化
    if(q6_first_flag == 0)
    {
        encoder_odometry_flag = 1;
        q6_first_flag = 1;
    }

    // 执行循迹控制
    if(turn_flag)
    {
        TB_Control(-250, 750);
        u8 x1,x2,x3,x4,x5,x6,x7,x8;
        deal_IRdata(&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8);
        if(x1 == 1 && x2 == 1 && x4 == 0 && x5 == 0)
        {
            turn_flag = 0;
            q6_turn_num ++;
        }
    }
    else LineWalking();
    // 转弯次数≥4时，完成1圈
    if(q6_turn_num >= 4)
    {
        q6_circle_num++;
        q6_turn_num = 0;
    }

    // 完成1圈后结束任务
    if(q6_circle_num >= q6_circle_N)
    {
        bee_time = 400;
        State_Machine.Main_State = STOP_STATE;
        State_Machine.Q6_State = STOP_STATE;

        q6_first_flag = 0;
        encoder_odometry_flag = 0;
        odometry_sum = 0;
    }
}