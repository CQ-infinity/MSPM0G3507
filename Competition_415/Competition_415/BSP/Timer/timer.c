#include "timer.h"

volatile uint32_t systick_counter = 0;


void TIMER_0_INST_IRQHandler(void)
{
    switch( DL_TimerG_getPendingInterrupt(TIMER_0_INST) )
    {
        case DL_TIMER_IIDX_ZERO://�����0����ж�  If it is a 0 overflow interrupt
            Buzzer_Handle();
            systick_counter++; // ÿ1ms�Զ�+1      +1 per sencond
            break;

        default:
            break;
    }
    
}

uint32_t Get_Time(void)    
{
    return systick_counter;
}









