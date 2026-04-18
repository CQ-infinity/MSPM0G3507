#include "delay.h"  // 包含延时功能相关的头文件，可能声明了本文件中的函数等

// 使用滴答定时器(SysTick)实现的精确微秒级延时函数
// 参数__us：需要延时的微秒数
void delay_us(unsigned long __us) 
{
    uint32_t ticks;        // 存储需要的总时钟周期数
    uint32_t told, tnow;   // 分别存储上一时刻和当前的SysTick计数值
    uint32_t tcnt = 38;    // 初始补偿值，用于抵消函数本身的执行耗时（经验值）

    // 计算需要的总时钟周期数：
    // 系统时钟频率为80MHz（80,000,000Hz），即每微秒(1/1,000,000秒)包含80个时钟周期
    // 因此总时钟周期 = 延时微秒数 × 每微秒的时钟周期数
    ticks = __us * (80000000 / 1000000);

    // 获取当前SysTick定时器的计数值（SysTick是递减计数器，从LOAD值减到0）
    told = SysTick->VAL;

    while (1)  // 循环等待，直到达到所需的延时
    {
        // 不断更新获取当前的SysTick计数值
        tnow = SysTick->VAL;

        if (tnow != told)  // 计数值发生变化时才进行计算（避免无效循环）
        {
            // 计算两次获取计数值之间经过的时钟周期数：
            // 情况1：tnow < told → 未发生溢出，直接用told - tnow
            // 情况2：tnow > told → 发生溢出（计数器从0重载为LOAD值），需计算两段差值之和
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;  // 更新上一时刻的计数值为当前值

            // 当累计的时钟周期数达到或超过所需的总周期数时，退出循环（延时完成）
            if (tcnt >= ticks)
                break;
        }
    }
}

// 使用滴答定时器实现的精确毫秒级延时函数
// 参数ms：需要延时的毫秒数
void delay_ms(unsigned long ms) 
{
    // 毫秒转微秒（1毫秒=1000微秒），直接调用微秒延时函数实现
    delay_us( ms * 1000 );
}