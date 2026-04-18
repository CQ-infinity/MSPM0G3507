#include "app_motor_usart.h"  // 包含电机串口通信应用层头文件（声明了串口通信相关函数、变量等）
#include "tb6612.h"

#define RXBUFF_LEN 256  // 接收缓冲区长度（256字节，用于存储串口接收的电机反馈数据）

// 全局变量：用于串口通信的数据存储
uint8_t send_buff[50];  // 发送缓冲区（50字节，用于构造发送给电机驱动板的指令）

float g_Speed[4];       // 四个电机的当前速度值（浮点数，单位根据协议确定）
int Encoder_Offset[4];  // 四个电机编码器的偏移值（可能是相对位移量）
int Encoder_Now[4];     // 四个电机编码器的当前值（绝对位置或累计脉冲数）

uint8_t g_recv_flag;    // 接收完成标志（0：未完成，1：已接收到完整有效数据）
uint8_t g_recv_buff[RXBUFF_LEN];  // 接收缓冲区（临时存储串口接收到的字节）
uint8_t g_recv_buff_deal[RXBUFF_LEN];  // 处理后的接收缓冲区（仅存储符合协议的有效数据）


//////////********************发送部分********************///////////
//////////******************Sending part*****************///////////

/**
 * @brief 发送电机类型指令（配置电机驱动板的电机型号）
 * @param data 电机类型（motor_type_t枚举值，对应不同电机型号）
 * @note 指令格式："$mtype:数据#"（以$开头，#结尾，中间为键值对）
 */
void send_motor_type(motor_type_t data)
{
    // 构造指令字符串（格式：$mtype:类型值#）
    sprintf((char*)send_buff,"$mtype:%d#",data);
    // 通过串口发送指令（发送缓冲区内容，长度为字符串实际长度）
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送电机死区指令（配置电机的最小驱动阈值，低于此值电机不响应）
 * @param data 死区值（整数，单位根据电机驱动板要求确定）
 * @note 指令格式："$deadzone:数据#"
 */
void send_motor_deadzone(uint16_t data)
{
    sprintf((char*)send_buff,"$deadzone:%d#",data);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送电机磁环线数指令（配置电机磁环的线数参数，用于编码器计算）
 * @param data 磁环线数（整数，参考电机手册）
 * @note 指令格式："$mline:数据#"
 */
void send_pulse_line(uint16_t data)
{
    sprintf((char*)send_buff,"$mline:%d#",data);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送电机减速比指令（配置电机的减速比参数，用于速度/距离计算）
 * @param data 减速比（整数，参考电机手册）
 * @note 指令格式："$mphase:数据#"
 */
void send_pulse_phase(uint16_t data)
{
    sprintf((char*)send_buff,"$mphase:%d#",data);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送车轮直径指令（配置车轮直径，用于里程计算）
 * @param data 车轮直径（浮点数，单位通常为毫米mm）
 * @note 指令格式："$wdiameter:数据#"（保留3位小数）
 */
void send_wheel_diameter(float data)
{
    sprintf((char*)send_buff,"$wdiameter:%.3f#",data);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送电机PID参数指令（配置电机速度/位置环的PID参数）
 * @param P 比例系数，I 积分系数，D 微分系数
 * @note 指令格式："$mpid:P,I,D#"（每个参数保留3位小数）
 */
void send_motor_PID(float P,float I,float D)
{
    sprintf((char*)send_buff,"$mpid:%.3f,%.3f,%.3f#",P,I,D);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送数据上传开关指令（控制电机驱动板是否反馈指定数据）
 * @param ALLEncoder_Switch 总编码器数据上传开关（true：开启，false：关闭）
 * @param TenEncoder_Switch 10ms编码器数据上传开关
 * @param Speed_Switch 速度数据上传开关
 * @note 指令格式："$upload:开关1,开关2,开关3#"
 */
void send_upload_data(bool ALLEncoder_Switch,bool TenEncoder_Switch,bool Speed_Switch)
{
    sprintf((char*)send_buff,"$upload:%d,%d,%d#",ALLEncoder_Switch,TenEncoder_Switch,Speed_Switch);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}

/**
 * @brief 发送速度控制指令（控制四个电机的目标速度）
 * @param M1_speed~M4_speed 分别为1~4号电机的目标速度（整数，单位根据协议确定）
 * @note 指令格式："$spd:M1,M2,M3,M4#"
 */
void Contrl_Speed(int16_t M1_speed,int16_t M2_speed,int16_t M3_speed,int16_t M4_speed)
{
    // sprintf((char*)send_buff,"$spd:%d,%d,%d,%d#",M1_speed,M2_speed,M3_speed,M4_speed);
    // Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
    TB_Control(M1_speed,  M3_speed);
}

/**
 * @brief 发送PWM控制指令（直接控制四个电机的PWM占空比）
 * @param M1_pwm~M4_pwm 分别为1~4号电机的PWM值（整数，范围通常为0~最大PWM值）
 * @note 指令格式："$pwm:M1,M2,M3,M4#"
 */
void Contrl_Pwm(int16_t M1_pwm,int16_t M2_pwm,int16_t M3_pwm,int16_t M4_pwm)
{
    sprintf((char*)send_buff,"$pwm:%d,%d,%d,%d#",M1_pwm,M2_pwm,M3_pwm,M4_pwm);
    Send_Motor_ArrayU8(send_buff, strlen((char*)send_buff));
}


//////////********************接收部分********************///////////
//////////*****************Receiving part****************///////////

// /**
//  * @brief 字符串分割函数（将字符串按指定分隔符拆分，存储到指针数组）
//  * @param mystrArray 存储分割后子字符串的指针数组
//  * @param str 要分割的原始字符串
//  * @param delimiter 分隔符（如","、" "等）
//  * @note 例如："123,456,789"按","分割后，指针数组依次指向"123"、"456"、"789"
//  */
// void splitString(char* mystrArray[],char *str, const char *delimiter) 
// {
//     // 第一次分割：从原始字符串开始，按分隔符拆分
//     char *token = strtok(str, delimiter); 
//     mystrArray[0] = token;  // 保存第一个分割后的子字符串
//     int i = 1;  // 从索引1开始存储后续子字符串
	
//     // 循环分割剩余部分（strtok(NULL, ...)表示继续处理上一次的字符串）
//     while (token != NULL) 
//     {
//         token = strtok(NULL, delimiter);
//         mystrArray[i] = token;  // 存储每个分割后的子字符串
//         i++;
//     }
// }

// /**
//  * @brief 处理接收到的单个字节（解析电机驱动板发送的数据，验证通信协议）
//  * @param rxtemp 接收到的单个字节
//  * @note 通信协议：数据以"$"开头，以"#"结尾，中间为有效数据；不符合协议则丢弃
//  */
// void Deal_Control_Rxtemp(uint8_t rxtemp)
// {
//     static u16 step = 0;  // 静态变量：当前接收缓冲区的索引（记录已接收的字节数）
//     static u8 start_flag = 0;  // 静态变量：起始标志（0：未检测到开头，1：已检测到"$"）

//     // 检测到起始符"$"且未开始接收时，标记开始接收并清空缓冲区
//     if(rxtemp == '$' && start_flag == 0)
//     {
//         start_flag = 1;  // 标记开始接收
//         memset(g_recv_buff,0,RXBUFF_LEN);  // 清空接收缓冲区
//     }
//     // 已开始接收（检测到"$"后）
//     else if(start_flag == 1)
//     {
//         // 检测到结束符"#"，表示一帧数据接收完成
//         if(rxtemp == '#')
//         {
//             start_flag = 0;  // 重置起始标志
//             step = 0;  // 重置索引
//             g_recv_flag = 1;  // 设置接收完成标志
//             // 将接收缓冲区的数据复制到处理缓冲区（仅保留有效数据）
//             memcpy(g_recv_buff_deal,g_recv_buff,RXBUFF_LEN); 
// //            printf("g_recv_buff_deal = %X\r\n",g_recv_buff_deal);  // 调试打印（当前注释）
//         }
//         // 未检测到结束符，继续存储字节
//         else
//         {
//             // 若接收字节数超过缓冲区长度，视为无效数据，重置状态
//             if(step > RXBUFF_LEN)
//             {
//                 start_flag = 0;  // 重置起始标志
//                 step = 0;  // 重置索引
//                 memset(g_recv_buff,0,RXBUFF_LEN);  // 清空接收缓冲区
//             }
//             // 否则正常存储字节
//             else
//             {
//                 g_recv_buff[step] = rxtemp;  // 存储当前字节到接收缓冲区
//                 step++;  // 索引递增
//             }
//         }
//     }
// }

// /**
//  * @brief 处理接收到的有效数据（解析电机驱动板反馈的编码器、速度等数据，转换为可用格式）
//  * @note 根据数据前缀区分数据类型（总编码器、10ms实时编码器、速度），解析后存储到全局变量
//  */
// void Deal_data_real(void)
// {
//     static uint8_t data[RXBUFF_LEN];  // 静态数组：临时存储提取的有效数据
//     uint8_t length = 0;  // 有效数据的长度

//     // 处理总编码器数据（前缀为"MAll"，格式："MAll:值1,值2,值3,值4"）
//     if ((strncmp("MAll",(char*)g_recv_buff_deal,4)==0))
//     {
//         // 计算有效数据长度（总长度减去前缀"MAll:"的5个字符）
//         length = strlen((char*)g_recv_buff_deal)-5;
//         // 提取":"后的有效数据（去除前缀）
//         for (uint8_t i = 0; i < length; i++)
//         {
//             data[i] = g_recv_buff_deal[i+5];  // 从索引5开始复制（跳过"MAll:"）
//         }  
//         data[length] = '\0';  // 添加字符串结束符

//         // 分割字符串并解析为四个电机的当前编码器值
//         char* strArray[10];  // 指针数组：存储分割后的子字符串
//         char mystr_temp[4][10] = {'\0'};  // 临时数组：存储每个编码器值的字符串
//         splitString(strArray,(char*)data, ", ");  // 按", "分割字符串
//         // 将分割后的字符串转换为整数，存储到Encoder_Now数组
//         for (int i = 0; i < 4; i++)
//         {
//             strcpy(mystr_temp[i],strArray[i]);  // 复制子字符串到临时数组
//             Encoder_Now[i] = atoi(mystr_temp[i]);  // 字符串转整数，存储当前编码器值
//         }
//     }
//     // 处理10ms实时编码器数据（前缀为"MTEP"，格式："MTEP:值1,值2,值3,值4"）
//     else if ((strncmp("MTEP",(char*)g_recv_buff_deal,4)==0))
//     {
//         length = strlen((char*)g_recv_buff_deal)-5;  // 计算有效数据长度（减去"MTEP:"）
//         // 提取":"后的有效数据
//         for (uint8_t i = 0; i < length; i++)
//         {
//             data[i] = g_recv_buff_deal[i+5];  // 跳过"MTEP:"
//         }  
//         data[length] = '\0';  // 添加结束符

//         // 分割字符串并解析为四个电机的编码器偏移值
//         char* strArray[10];
//         char mystr_temp[4][10] = {'\0'}; 
//         splitString(strArray,(char*)data, ", ");  // 按", "分割
//         // 转换为整数，存储到Encoder_Offset数组
//         for (int i = 0; i < 4; i++)
//         {
//             strcpy(mystr_temp[i],strArray[i]);
//             Encoder_Offset[i] = atoi(mystr_temp[i]);  // 字符串转整数，存储偏移值
//         }
//     }
//     // 处理速度数据（前缀为"MSPD"，格式："MSPD:值1,值2,值3,值4"）
//     else if ((strncmp("MSPD",(char*)g_recv_buff_deal,4)==0))
//     {
//         length = strlen((char*)g_recv_buff_deal)-5;  // 计算有效数据长度（减去"MSPD:"）
//         // 提取":"后的有效数据
//         for (uint8_t i = 0; i < length; i++)
//         {
//             data[i] = g_recv_buff_deal[i+5];  // 跳过"MSPD:"
//         }  
//         data[length] = '\0';  // 添加结束符

//         // 分割字符串并解析为四个电机的速度值
//         char* strArray[10];
//         char mystr_temp[4][10] = {'\0'}; 
//         splitString(strArray,(char*)data, ", ");  // 按", "分割
//         // 转换为浮点数，存储到g_Speed数组
//         for (int i = 0; i < 4; i++)
//         {
//             strcpy(mystr_temp[i],strArray[i]);
//             g_Speed[i] = atof(mystr_temp[i]);  // 字符串转浮点数，存储速度值
//         }
//     }
// }