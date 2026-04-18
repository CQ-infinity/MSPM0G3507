#ifndef _APP_IRTRACKING_H_
#define _APP_IRTRACKING_H_


#include "ti_msp_dl_config.h"
#include "app_motor.h"
#include "usart.h"
#include "No_Mcu_Ganv_Grayscale_Sensor_Config.h"
#include "timer.h"          // 定时器头文件（用于时间相关的控制或延时）

#define BLACK       1        //黑线black
#define WHITE       0        //白线white


#define u8 uint8_t
#define u16 uint16_t

#define IR_I2C_ADDR 0x12

void IRI2C_WriteByte(uint8_t addr, uint8_t dat);
uint8_t IRI2C_ReadByte(uint8_t addr);
void printf_i2c_data(void);
void LineWalking(void);
void New_LineWalking(int turn);
int LineCheck(void);

void deal_IRdata(u8 *x1,u8 *x2,u8 *x3,u8 *x4,u8 *x5,u8 *x6,u8 *x7,u8 *x8);


// 全局变量定义
 extern unsigned short Anolog[8];  // 存储8路传感器的原始模拟量数据
// 黑白校准参考值（初始化为默认值，可通过实际校准调整）
 extern unsigned short white[8];  // 白色表面的参考值
 extern unsigned short black[8];          // 黑色表面的参考值
 extern unsigned short Normal[8];        // 存储8路传感器的归一化数据（基于黑白参考值计算）
 extern unsigned char rx_buff[256];// 串口发送缓冲区（最大256字节）
// 局部变量定义
extern No_MCU_Sensor sensor;  // 灰度传感器设备结构体（存储传感器状态和配置）
extern unsigned char Digtal;  // 存储8路传感器的数字量结果（1字节，每bit代表1路状态）

#endif

