// 头文件包含：每个头文件对应特定功能模块的接口定义
#include "Debug/syscfg/ti_msp_dl_config.h"   // MSP430系列芯片底层配置头文件（包含时钟、GPIO、外设等硬件配置）
#include "delay.h"              // 延时功能模块（提供毫秒级/微秒级延时函数）
#include "usart.h"              // 串口通信模块（包含串口初始化、发送/接收等函数）
#include "app_motor_usart.h"    // 电机串口应用模块（通过串口实现电机参数配置等功能）
#include "app_motor.h"          // 电机应用模块（包含PWM输出、运动模式控制等函数）
#include "app_irtracking.h"     // 红外循迹应用模块（处理循迹传感器数据、实现循迹控制逻辑）
#include "key.h"                // 按键功能模块（包含按键检测、按键事件处理等函数）
#include "led.h"                // LED功能模块（实现LED点亮、闪烁等控制）
#include "buzzer.h"             // 蜂鸣器功能模块（实现蜂鸣器发声、频率控制等）
#include "task.h"               // 任务调度模块（实现多任务优先级管理、任务调度逻辑）
#include "questions.h"          // 任务题目模块（包含不同任务对应的处理逻辑）
#include "servo.h"              // 舵机控制模块（实现舵机角度控制等功能）
#include "No_Mcu_Ganv_Grayscale_Sensor_Config.h" // 无MCU灰度传感器配置模块（定义传感器数据结构、初始化和数据处理函数）


// 电机类型定义：根据不同电机参数匹配对应的控制配置
// 1:520电机  2:310电机  3:带编码器的TT电机  4:TT直流行星电机  5:520减速电机
#define MOTOR_TYPE 2   // 当前选择310电机


// 全局变量定义
int g_LinePortal_flag = 0;  // 循迹启动标志位：0-未启动循迹；1-已启动循迹（控制循迹逻辑是否执行）
char buffer[256];            // 串口数据缓冲区：用于存储串口接收/发送的字符串数据（支持格式化输出）

// 全局变量定义
 unsigned short Anolog[8] = {0};  // 存储8路传感器的原始模拟量数据
// 黑白校准参考值（初始化为默认值，可通过实际校准调整）
 unsigned short white[8] = {2500,2500,2500,2500,2500,2500,2500,2500};  // 白色表面的参考值
 unsigned short black[8] = {1000,1000,1000,1000,1000,1000,1000,1000};                        // 黑色表面的参考值
 unsigned short Normal[8];        // 存储8路传感器的归一化数据（基于黑白参考值计算）
 unsigned char rx_buff[256] = {0};// 串口发送缓冲区（最大256字节）
// 局部变量定义
No_MCU_Sensor sensor;  // 灰度传感器设备结构体（存储传感器状态和配置）
unsigned char Digtal;  // 存储8路传感器的数字量结果（1字节，每bit代表1路状态）


int main(void)
{
    USART_Init();  // 初始化串口（用于调试打印、数据上传、与上位机通信等）
    delay_ms(10);  // 延时10ms：等待串口初始化稳定

    // Set_Motor(MOTOR_TYPE);  // 根据电机类型配置电机参数（如PWM范围、转速转换系数等）
    // // 发送初始上传数据：参数分别表示"是否上传传感器数据"、"是否上传状态数据"、"是否上传调试数据"
    // send_upload_data(false, true, false);  
    // delay_ms(10);  // 延时10ms：等待数据发送完成


    PWM_Buzzer_Init();    // 初始化PWM蜂鸣器（配置PWM定时器，用于控制蜂鸣器频率）
    State_Machine_init(); // 初始化状态机（用于管理系统不同工作状态的切换逻辑）


    // 配置定时器中断（用于定时触发任务，如周期性数据采集、控制逻辑执行）
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);  // 清除定时器0中断挂起标志（避免初始化前的无效中断）
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);        // 使能定时器0中断（允许定时器触发中断）
    DL_TimerA_startCounter(TIMER_0_INST);          // 启动定时器0计数（开始计时）

    delay_ms(500);
//注意！！！！下面是传感器初始化

    // memset(Anolog, 0, sizeof(Anolog));
    // memset(white, 0, sizeof(white));
    // memset(black, 0, sizeof(black));
    // memset(Normal, 0, sizeof(Normal));
    // memset(rx_buff, 0, sizeof(rx_buff));
    // while(1){
    // 传感器首次初始化（不带黑白校准值，用于获取原始数据进行校准）
    No_MCU_Ganv_Sensor_Init_Frist(&sensor);
    // 执行传感器任务（无时基版本，不依赖定时器，直接读取数据）
    No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor);  //完成数据采集，数据二值化，数据归一化
    // 获取传感器原始模拟量数据，存储到Anolog数组
    Get_Anolog_Value(&sensor, Anolog);
    //传感器使用前要进行初始化适应环境
    // 打印初始模拟量数据（用于校准黑白参考值）
    // 此时输出的是传感器在当前环境下的原始AD值，用户可根据实际黑白表面的读数调整white/black数组
    // 也可扩展按键逻辑实现一键校准（自动将当前值写入white或black数组）
    sprintf(buffer, "Anolog %d-%d-%d-%d-%d-%d-%d-%d\r\n",
            Anolog[0], Anolog[1], Anolog[2], Anolog[3],
            Anolog[4], Anolog[5], Anolog[6], Anolog[7]);
    USART_SendString(buffer);  // 通过串口0发送数据
    delay_ms(1000);                       // 延时100ms，避免数据发送过快
    // }
    // 使用校准后的黑白参考值重新初始化传感器（此时传感器可进行数字量和归一化计算）
    No_MCU_Ganv_Sensor_Init(&sensor, white, black);

    delay_ms(100);  // 初始化后延时，确保传感器稳定


    // 初始化成功提示：LED闪烁4次，蜂鸣器同步响4次，指示系统初始化完成
    int i = 0;
    for(i = 0; i < 4; i++)
    {
        LED2_Toggle();    // 切换LED2状态（亮/灭交替）
        Buzzer_Toggle();  // 切换蜂鸣器状态（响/停交替）
        delay_ms(100);    // 每次状态切换间隔100ms
    }

    while(1)  // 主循环：系统核心逻辑执行
    {
        Scheduler_Run();  // 运行任务调度器（按优先级执行各任务，如传感器数据读取、状态检测等）

        // 若循迹启动标志位为1（已启动循迹），则根据当前主状态执行对应逻辑
        if(g_LinePortal_flag)
        {    
            // 根据状态机的主状态执行对应任务
            switch(State_Machine.Main_State)
            {
                case STOP_STATE:          // 停止状态：所有电机停止
                    TB_Control(0,0);  // 设置4个电机的PWM值为0（停止转动）
                    break;
                
                case QUESTION_1:         // 任务1状态：执行任务1的处理逻辑
                    Question_Task_1();   // 任务1具体实现（例如：特定路径循迹、速度控制等）
                    break;
                
                case QUESTION_2:         // 任务2状态：执行任务2的处理逻辑
                    // Question_Task_2();   // 任务2具体实现（当前注释，未启用）
                    break;
                
                case QUESTION_3:         // 任务3状态：执行任务3的处理逻辑
                    // Question_Task_3();   // 任务3具体实现（当前注释，未启用）
                    break;
                
                case QUESTION_4:         // 任务4状态：执行任务4的处理逻辑
                    Question_Task_4();   // 任务4具体实现
                    break;

                case QUESTION_5:         // 任务5状态：执行任务5的处理逻辑
                    Question_Task_5();   // 任务5具体实现
                    break;

                case QUESTION_6:         // 任务6状态：执行任务6的处理逻辑
                    Question_Task_6();   // 任务6具体实现
                    break;
                
                default:                 // 默认状态：无操作
                    break;
            }    
        }
    }
}