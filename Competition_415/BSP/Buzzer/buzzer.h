#ifndef	__BUZZER_H__
#define __BUZZER_H__

#include "ti_msp_dl_config.h"
#include "delay.h"
#include "led.h"

extern volatile uint32_t bee_time;           //蜂鸣器鸣叫的时间（单位：ms） the sound_time of beep(the unit is ms)

void Buzzer_Handle(void);
void PWM_Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Beep_Times(int times);
void Buzzer_Toggle(void);

#endif
