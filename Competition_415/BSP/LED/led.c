#include "led.h"

void LED_Toggle(void)
{
    DL_GPIO_togglePins(LED_PORT, LED_D1_PIN);
}

void LED_ON(void)
{
    DL_GPIO_setPins(LED_PORT,LED_D1_PIN);  //LED控制输出高电平  LED control output high level
}

void LED_OFF(void)
{
    DL_GPIO_clearPins(LED_PORT,LED_D1_PIN);//LED控制输出低电平  LED control output low level
}

void LED2_Toggle(void)
{
    DL_GPIO_togglePins(LED_PORT, LED_D2_PIN);
}
