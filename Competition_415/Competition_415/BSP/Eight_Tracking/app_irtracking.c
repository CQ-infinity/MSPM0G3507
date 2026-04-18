#include "app_irtracking.h"  // 红外循迹应用层头文件（包含循迹控制逻辑、参数定义等）

// 红外循迹转向PID参数（用于控制转向的比例、积分、微分系数）
#define IRTrack_Trun_KP (120)  // 比例系数（影响响应速度，值越大转向越灵敏）
#define IRTrack_Trun_KI (0)    // 积分系数（当前未启用，用于消除静态误差）
#define IRTrack_Trun_KD (8)    // 微分系数（影响稳定性，减小超调量）

int pid_output_IRR = 0;       // 红外循迹PID计算的输出值（用于控制小车转向）
u8 trun_flag = 0;             // 转向标志位（预留，用于区分不同转向状态）

char buffer[200];             // 串口打印缓冲区（用于调试信息输出）


#define IRR_SPEED 200  // 循迹基础速度（小车前进的基准速度，具体值由硬件逻辑确定）

/**
 * @brief 红外循迹PID计算函数（位置式PID）
 * @param actual_value 实际偏差值（由红外传感器检测到的轨迹偏移量）
 * @return 计算得到的PID输出值（用于控制小车转向）
 */
float APP_ELE_PID_Calc(int8_t actual_value)
{
    float IRTrackTurn = 0;       // PID计算的转向控制量
    int8_t error;                // 当前误差（目标值与实际值的差值，此处直接使用实际偏差）
    static int8_t error_last = 0;// 上一次的误差（用于计算微分项）
    static float IRTrack_Integral; // 积分项（累计误差，用于消除静态偏差）
    
    error = actual_value;        // 赋值当前误差（红外检测到的偏移量）
    
    IRTrack_Integral += error;   // 累加误差（计算积分项）
    
    // 位置式PID公式：输出 = 比例项 + 积分项 + 微分项
    IRTrackTurn = error * IRTrack_Trun_KP          // 比例项（当前误差×比例系数）
                + IRTrack_Trun_KI * IRTrack_Integral  // 积分项（积分系数×累计误差）
                + (error - error_last) * IRTrack_Trun_KD; // 微分项（微分系数×误差变化量）
    
    error_last = error;          // 保存当前误差，用于下一次计算微分项
    return IRTrackTurn;          // 返回PID计算结果（转向控制量）
}


/**
 * @brief 通过I2C向红外循迹传感器写入一个字节数据
 * @param addr 传感器内部寄存器地址（要写入数据的目标地址）
 * @param dat 要写入的数据
 */
void IRI2C_WriteByte(uint8_t addr, uint8_t dat) {
    uint8_t temp[2];  // 临时数组：存储地址和数据（I2C通信需先发送地址再发送数据）
    temp[0] = addr;   // 第一个元素存寄存器地址
    temp[1] = dat;    // 第二个元素存要写入的数据
 
    // 填充I2C控制器发送FIFO，将temp数组的数据写入发送缓冲区
    DL_I2C_fillControllerTXFIFO(Sensor_INST, temp, 2);
    // 等待I2C控制器空闲（确保前一次通信已完成）
    while (!(DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
 
    // 启动I2C传输：目标设备地址IR_I2C_ADDR，发送方向，传输2字节（地址+数据）
    DL_I2C_startControllerTransfer(Sensor_INST, IR_I2C_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    // 等待I2C总线空闲（避免总线冲突）
    while (DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    // 等待I2C传输完成并回到空闲状态
    while (!(DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    // 清空发送FIFO（避免残留数据影响下一次通信）
    DL_I2C_flushControllerTXFIFO(Sensor_INST);
}
 
/**
 * @brief 通过I2C从红外循迹传感器读取一个字节数据
 * @param addr 要读取的寄存器地址
 * @return 从指定地址读取的数据
 */
uint8_t IRI2C_ReadByte(uint8_t addr) {
    uint8_t data;  // 存储读取到的数据
 
    // 填充发送FIFO：先发送要读取的寄存器地址（告知传感器读取目标）
    DL_I2C_fillControllerTXFIFO(Sensor_INST, &addr, 1);
	
	// 禁用I2C发送FIFO触发中断（避免读取过程中被中断干扰）
	DL_I2C_disableInterrupt(Sensor_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
	
    // 等待I2C控制器空闲
    while (!(DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    // 启动I2C发送：发送1字节（寄存器地址）
    DL_I2C_startControllerTransfer(Sensor_INST, IR_I2C_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    // 等待总线空闲
    while (DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    // 等待地址发送完成并回到空闲状态
    while (!(DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
 
    // 启动I2C接收：从传感器读取1字节数据
    DL_I2C_startControllerTransfer(Sensor_INST, IR_I2C_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, 1);
    // 等待总线空闲
    while (DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    // 等待读取完成并回到空闲状态
    while (!(DL_I2C_getControllerStatus(Sensor_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    // 从接收缓冲区读取数据
    data = DL_I2C_receiveControllerData(Sensor_INST);
 
    return data;  // 返回读取到的数据
}


/**
 * @brief 解析红外传感器数据，获取8个红外探头的状态
 * @param x1~x8 用于存储8个红外探头状态的指针（0表示检测到黑线，1表示检测到白线）
 */
void deal_IRdata(u8 *x1,u8 *x2,u8 *x3,u8 *x4,u8 *x5,u8 *x6,u8 *x7,u8 *x8)
{
    u8 IRbuf = 0xFF;  // 存储从传感器读取的原始数据（1字节，8位对应8个探头）
    //读取当前传感器的数据
    No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor); // 执行传感器任务（无时基版本）：更新模拟量、计算数字量和归一化值

    // 读取传感器数据寄存器（0x30为红外数据寄存器地址）
    // IRbuf = IRI2C_ReadByte(0x30);
    IRbuf = Get_Digtal_For_User(&sensor);  
    
    // 通过位运算提取8个探头的状态（从高位到低位对应x1到x8）
    *x1 = (IRbuf >> 0) & 0x01;  // 取第7位（最高位）作为x1的状态
    *x2 = (IRbuf >> 1) & 0x01;  // 取第6位作为x2的状态
    *x3 = (IRbuf >> 2) & 0x01;  // 取第5位作为x3的状态
    *x4 = (IRbuf >> 3) & 0x01;  // 取第4位作为x4的状态
    *x5 = (IRbuf >> 4) & 0x01;  // 取第3位作为x5的状态
    *x6 = (IRbuf >> 5) & 0x01;  // 取第2位作为x6的状态
    *x7 = (IRbuf >> 6) & 0x01;  // 取第1位作为x7的状态
    *x8 = (IRbuf >> 7) & 0x01;  // 取第0位（最低位）作为x8的状态
}


/**
 * @brief 打印红外传感器数据（调试用，输出8个红外探头的状态）
 */
void printf_i2c_data(void)
{
    static uint8_t ir_x1,ir_x2,ir_x3,ir_x4,ir_x5,ir_x6,ir_x7,ir_x8;  // 存储8个探头状态
    deal_IRdata(&ir_x1,&ir_x2,&ir_x3,&ir_x4,&ir_x5,&ir_x6,&ir_x7,&ir_x8);  // 解析传感器数据
    // 串口打印8个探头的状态（x1到x8）
    sprintf(buffer, "x1:%d,x2:%d,x3:%d,x4:%d,x5:%d,x6:%d,x7:%d,x8:%d\r\n",
           ir_x1,ir_x2,ir_x3,ir_x4,ir_x5,ir_x6,ir_x7,ir_x8);
           USART_SendString(buffer);
}



/**
 * @brief 红外循迹控制逻辑（版本1）
 * 功能：根据红外探头检测到的轨迹偏移，计算转向控制量，实现小车循迹
 */
void LineWalking(void)
{
    // 定义存储实时探头状态和控制所需的变量
    u8 x1, x2, x3, x4, x5, x6, x7, x8;  // 8路探头状态（0=黑线，1=白线）
    int err = 0;                        // 实时误差（直接映射为转向控制量）
    static int last_err = 0;            // 上一次误差（用于PID微分计算）

    // 1. 读取探头实时状态
    deal_IRdata(&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8);
    // printf_i2c_data();

    // 2. 定义权重数组（左右探头权重对称，用于量化偏移程度）
    // 左侧探头（x1-x4）权重为负，右侧（x5-x8）为正，值越大对转向影响越显著
    const int weight[8] = {-20, -15, -10, -5, 5, 10, 15, 20};

    // 3. 计算实时误差（累加检测到黑线的探头权重）
    if (x1 == 0) err += weight[0];  // x1检测到黑线，累加对应权重
    if (x2 == 0) err += weight[1];
    if (x3 == 0) err += weight[2];
    if (x4 == 0) err += weight[3];
    if (x5 == 0) err += weight[4];
    if (x6 == 0) err += weight[5];
    if (x7 == 0) err += weight[6];
    if (x8 == 0) err += weight[7];

    last_err = err;  // 保存当前误差为历史误差

    // 4. 计算PID输出（转向控制量）
    pid_output_IRR = (int)(APP_ELE_PID_Calc(err));
    
    Motion_Car_Control(640, 0, pid_output_IRR);

    if(x1 == 0 && x2 == 0)
    {
        turn_flag = 1;
    }

}

// /**
//  * @brief 红外循迹控制逻辑（版本2，优化版）
//  * 功能：通过红外数据计算偏移误差，结合PID控制实现小车循迹，支持转向限制
//  * @param turn 转向限制标志（0：限制向右偏移；1：限制向左偏移）
//  */
// void New_LineWalking(int turn)       
// {
//     static int8_t err = 0;  // 循迹误差（用于PID计算，负值表示左偏，正值表示右偏）
//     static u8 x1, x2, x3, x4, x5, x6, x7, x8;  // 存储8个红外探头状态
    
//     // 读取8个红外探头的实时状态
//     deal_IRdata(&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8);

//     // 定义探头权重数组（左侧为负，右侧为正，量化偏移程度）
//     // 权重绝对值反映探头对转向的影响强度（越靠近边缘的探头权重越大）
//     const int8_t sensor_weight[8] = {-7, -6, -4, -2, 2, 4, 6, 7};
//     int sum_weight = 0;  // 检测到黑线的探头权重总和
//     int black_count = 0; // 检测到黑线的探头数量（用于计算平均误差）
    
//     // 累加黑线探头的权重，并统计数量
//     if (x1 == 0) { sum_weight += sensor_weight[0]; black_count++; }
//     if (x2 == 0) { sum_weight += sensor_weight[1]; black_count++; }
//     if (x3 == 0) { sum_weight += sensor_weight[2]; black_count++; }
//     if (x4 == 0) { sum_weight += sensor_weight[3]; black_count++; }
//     if (x5 == 0) { sum_weight += sensor_weight[4]; black_count++; }
//     if (x6 == 0) { sum_weight += sensor_weight[5]; black_count++; }
//     if (x7 == 0) { sum_weight += sensor_weight[6]; black_count++; }
//     if (x8 == 0) { sum_weight += sensor_weight[7]; black_count++; }

//     // 若检测到黑线，计算平均误差（权重总和/探头数量，降低单探头干扰）
//     if (black_count > 0) 
//     {
//         err = sum_weight / black_count;  // 误差 = 总权重 ÷ 黑线探头数量（平滑误差）
//     }
    
//     // 初始角度校准（仅执行一次）：根据误差调整初始导航角度
//     if(adjustFirstAngle){
//         first_angle=navigetion_0_360_limit(first_angle+err*1.0);
//         adjustFirstAngle =0;
//     }
    
//     // 根据转向限制标志过滤误差（限制特定方向的偏移）
//     if(turn == 1 && err < 0) err = 0;     // 限制向左转向（turn=1时，忽略左偏误差）
//     if(turn == 0 && err > 0) err = 0;     // 限制向右转向（turn=0时，忽略右偏误差）

//     // 计算PID输出（转向控制量）
//     pid_output_IRR = (int)(APP_ELE_PID_Calc(err));
    
//     // 控制小车运动：基础速度为IRR_SPEED减去与误差相关的量（转向时减速，提高稳定性）
//     // 第三个参数为转向控制量（PID输出）
//     Motion_Car_Control(IRR_SPEED - 3*abs(err), 0, pid_output_IRR);

//     // 以下为调试用代码（注释掉）：打印时间、左右轮速度
//     // float vL = IRR_SPEED - 3*abs(err) + 0.188*pid_output_IRR;
//     // float vR = IRR_SPEED - 3*abs(err) - 0.188*pid_output_IRR;
//     // sprintf(buffer, "t=%d, vL=%f, vR=%f\r\n", Get_Time(), vL, vR);
//     // USART_SendString(buffer);
// }


/**
 * @brief 检测当前是否在黑线上（区分黑线和白线）
 * @return BLACK：检测到黑线；WHITE：未检测到黑线（在白线上）
 * @note 只要有一个红外探头检测到黑线（x=0），即判定为在黑线上
 */
int LineCheck(void)
{
    int if_have = 0;  // 是否检测到黑线（0：未检测到；1：检测到）
    static u8 x1,x2,x3,x4,x5,x6,x7,x8;  // 存储8个红外探头状态
    
    // 读取8个红外探头的状态
    deal_IRdata(&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8);
    
    // 检查是否有探头检测到黑线（x=0表示黑线，x=1表示白线）
    if(!x1) if_have = 1;  // x1检测到黑线
    if(!x2) if_have = 1;
    if(!x3) if_have = 1;
    if(!x4) if_have = 1;
    if(!x5) if_have = 1;
    if(!x6) if_have = 1;
    if(!x7) if_have = 1;
    if(!x8) if_have = 1;
        
    // 根据检测结果返回对应状态
    if(if_have >= 1)  // 至少一个探头检测到黑线
    {
        return BLACK;
    }
    else  // 所有探头均未检测到黑线（在白线上）
    {
        return WHITE;
    }
}