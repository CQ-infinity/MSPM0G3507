#include "servo_usart.h"

// #define SERVO_INST MOTOR_INST

#define RXBUFF_LEN 256  // 接收缓冲区长度（256字节，用于存储串口接收的电机反馈数据）

// 全局变量：用于串口通信的数据存储
uint8_t send_buff[50];  // 发送缓冲区（50字节，用于构造发送给电机驱动板的指令）

int Pos_Now[2];     //从摄像头传来的坐标
uint8_t g_recv_flag;    // 接收完成标志（0：未完成，1：已接收到完整有效数据）
uint8_t g_recv_buff[RXBUFF_LEN];  // 接收缓冲区（临时存储串口接收到的字节）
uint8_t g_recv_buff_deal[RXBUFF_LEN];  // 处理后的接收缓冲区（仅存储符合协议的有效数据）



void UART_1_INST_IRQHandler(void)
{
	uint8_t Rx2_Temp = 0;
	
	switch( DL_UART_getPendingInterrupt(SERVO_INST) )
	{
		case DL_UART_IIDX_RX://如果是接收中断	If it is a receive interrupt
			
			// 接收发送过来的数据保存	Receive and save the data sent
			Rx2_Temp = DL_UART_Main_receiveData(SERVO_INST);
			//处理	deal with
			Deal_Control_Rxtemp(Rx2_Temp);
			break;
		
		default://其他的串口中断	Other serial port interrupts
			break;
	}	
	

}


/**
 * @brief 字符串分割函数（将字符串按指定分隔符拆分，存储到指针数组）
 * @param mystrArray 存储分割后子字符串的指针数组
 * @param str 要分割的原始字符串
 * @param delimiter 分隔符（如","、" "等）
 * @note 例如："123,456,789"按","分割后，指针数组依次指向"123"、"456"、"789"
 */
void splitString(char* mystrArray[],char *str, const char *delimiter) 
{
    // 第一次分割：从原始字符串开始，按分隔符拆分
    char *token = strtok(str, delimiter); 
    mystrArray[0] = token;  // 保存第一个分割后的子字符串
    int i = 1;  // 从索引1开始存储后续子字符串
	
    // 循环分割剩余部分（strtok(NULL, ...)表示继续处理上一次的字符串）
    while (token != NULL) 
    {
        token = strtok(NULL, delimiter);
        mystrArray[i] = token;  // 存储每个分割后的子字符串
        i++;
    }
}

/**
 * @brief 处理接收到的单个字节（解析电机驱动板发送的数据，验证通信协议）
 * @param rxtemp 接收到的单个字节
 * @note 通信协议：数据以"$"开头，以"#"结尾，中间为有效数据；不符合协议则丢弃
 */
void Deal_Control_Rxtemp(uint8_t rxtemp)
{
    static uint16_t step = 0;  // 静态变量：当前接收缓冲区的索引（记录已接收的字节数）
    static uint8_t start_flag = 0;  // 静态变量：起始标志（0：未检测到开头，1：已检测到"$"）

    // 检测到起始符"$"且未开始接收时，标记开始接收并清空缓冲区
    if(rxtemp == '$' && start_flag == 0)
    {
        start_flag = 1;  // 标记开始接收
        memset(g_recv_buff,0,RXBUFF_LEN);  // 清空接收缓冲区
    }
    // 已开始接收（检测到"$"后）
    else if(start_flag == 1)
    {
        // 检测到结束符"#"，表示一帧数据接收完成
        if(rxtemp == '#')
        {
            start_flag = 0;  // 重置起始标志
            step = 0;  // 重置索引
            g_recv_flag = 1;  // 设置接收完成标志
            // 将接收缓冲区的数据复制到处理缓冲区（仅保留有效数据）
            memcpy(g_recv_buff_deal,g_recv_buff,RXBUFF_LEN); 
//            printf("g_recv_buff_deal = %X\r\n",g_recv_buff_deal);  // 调试打印（当前注释）
        }
        // 未检测到结束符，继续存储字节
        else
        {
            // 若接收字节数超过缓冲区长度，视为无效数据，重置状态
            if(step > RXBUFF_LEN)
            {
                start_flag = 0;  // 重置起始标志
                step = 0;  // 重置索引
                memset(g_recv_buff,0,RXBUFF_LEN);  // 清空接收缓冲区
            }
            // 否则正常存储字节
            else
            {
                g_recv_buff[step] = rxtemp;  // 存储当前字节到接收缓冲区
                step++;  // 索引递增
            }
        }
    }
}

/**
 * @brief 处理接收到的有效数据（解析电机驱动板反馈的编码器、速度等数据，转换为可用格式）
 * @note 根据数据前缀区分数据类型（总编码器、10ms实时编码器、速度），解析后存储到全局变量
 */
void Deal_data_real(void)
{
    static uint8_t data[RXBUFF_LEN];  // 静态数组：临时存储提取的有效数据
    uint8_t length = 0;  // 有效数据的长度

    if(strncmp("LXY",(char*)g_recv_buff_deal,3)==0)
    {
        // 屏幕坐标 320*240
        // 屏幕中心坐标 160*120
        // 靶心坐标相对于中心偏右 舵机向右转
        // 可以借鉴追踪色块的代码
        length = strlen((char*)g_recv_buff_deal)-4;
        for (uint8_t i = 0; i < length; i++)
        {
            data[i] = g_recv_buff_deal[i+4];  // 从索引4开始复制（跳过"LXY:"）
        }
        data[length] = '\0';  // 添加字符串结束符

        // 分割字符串并解析为四个电机的当前编码器值
        char* strArray[10];  // 指针数组：存储分割后的子字符串
        char mystr_temp[2][10] = {'\0'};  // 临时数组：存储每个编码器值的字符串
        splitString(strArray,(char*)data, ", ");  // 按", "分割字符串
        // 将分割后的字符串转换为整数，存储到Encoder_Now数组
        for (int i = 0; i < 2; i++)
        {
            strcpy(mystr_temp[i],strArray[i]);  // 复制子字符串到临时数组
            Pos_Now[i] = atoi(mystr_temp[i]);  // 字符串转整数，存储当前编码器值
        }
    }
}