#ifndef _KEY_H
#define _KEY_H

#include "ti_msp_dl_config.h"
#include "timer.h"
#include "led.h"
#include "usart.h"
#include "app_motor_usart.h"
#include "buzzer.h"
#include "questions.h"

/* 按键事件类型定义 - Key Event Type Definitions */
typedef enum {
    KEY_EVENT_NONE,     // 无事件 / No event
    KEY_EVENT_SHORT,    // 短按事件 / Short press event
    KEY_EVENT_LONG      // 长按事件 / Long press event
} KeyEvent;

/* 按键状态机状态 - Key State Machine States */
typedef enum {
    KEY_STATE_RELEASED,     // 按键释放状态 / Key is released
    KEY_STATE_DEBOUNCE,     // 消抖检测状态 / Debounce checking
    KEY_STATE_PRESSED,      // 确认按下状态 / Key is confirmed pressed
    KEY_STATE_LONG          // 长按已触发状态 / Long press triggered
} KeyState;

/* 按键句柄结构体 - Key Handle Structure */
typedef struct {
    GPIO_Regs* GPIOx;    // GPIO端口 / GPIO Port (e.g., GPIOA)
    uint32_t GPIO_Pin;      // GPIO引脚 / GPIO Pin (e.g., GPIO_PIN_0)
    KeyState state;         // 当前状态 / Current state
    uint32_t pressTime;     // 按下时间戳（单位：ms） / Press timestamp (ms)
    uint32_t debounceTime;  // 消抖计时起点（单位：ms） / Debounce start time (ms)
} Key_t;

KeyEvent Key_Scan(Key_t* key, uint32_t currentTime, uint32_t longPressThreshold);
void Key_Handle(void);

#endif