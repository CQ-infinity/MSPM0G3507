#include "buzzer.h"

void PWM_Buzzer_Init(void) {
    DL_Timer_startCounter(BUZZER_INST);
}

void Buzzer_Toggle(void)
{
    static int i=0;
    if(i==0)
    {
        Buzzer_ON();
        i=1;
    }
    else
    {
        Buzzer_OFF();
        i=0;
    }
}

void Buzzer_ON(void)
{
    DL_TimerA_setCaptureCompareValue(BUZZER_INST, 100, GPIO_BUZZER_C3_IDX);
}

void Buzzer_OFF(void)
{
    DL_TimerA_setCaptureCompareValue(BUZZER_INST, 0, GPIO_BUZZER_C3_IDX);
}

//蜂鸣器鸣叫的时间（单位：ms）
//the sound_time of beep(the unit is ms)
volatile uint32_t bee_time = 0;
//该功能运行在定时器中断中（1 毫秒）
//This function should be placed in the timer interrupt (1ms)
void Buzzer_Handle(void)
{
    static bool buzzer_state = false;
    if (bee_time > 0) 
    {
        if (!buzzer_state) 
        {
            LED_ON();
            Buzzer_ON();
            buzzer_state = true;
        }
        bee_time--;
    } 
    else 
    {
        if (buzzer_state) 
        {
            LED_OFF();
            Buzzer_OFF();
            buzzer_state = false;
        }
    }	
}

void Beep_Times(int times)
{
    int i = 0;
    for(i=0;i<times;i++)
    {
        Buzzer_ON();
        delay_ms(100);
        Buzzer_OFF();
        delay_ms(100);
    }
}

