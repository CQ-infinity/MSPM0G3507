#ifndef	__TASK_H__
#define __TASK_H__

#include "ti_msp_dl_config.h"
#include "app_motor.h"
#include "app_irtracking.h"
#include "key.h"
#include "led.h"
#include "buzzer.h"


typedef struct {
    uint32_t interval;    // ����ִ�м������λ��ms��   Task execution interval (unit: ms)
    uint32_t last_call;    // �ϴ�ִ��ʱ���    Last execution time stamp
    void (*task)(void);    // ������ָ��  Task function pointer
} Task;

void Scheduler_Run(void);

#endif
