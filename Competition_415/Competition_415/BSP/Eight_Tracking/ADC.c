#include "ADC.h"

char buffer[256];

unsigned int adc_getValue(void)
{
    unsigned int gAdcResult = 0;

    //使能ADC转换
    DL_ADC12_enableConversions(ADC1_INST);
    //软件触发ADC开始转换
    DL_ADC12_startConversion(ADC1_INST);

    //如果当前状态 不是 空闲状态
    int i = 0;
    while (DL_ADC12_getStatus(ADC1_INST) != DL_ADC12_STATUS_CONVERSION_IDLE )
    {
        i++;
        // sprintf(buffer, "%d\r\n", i);
        // USART_SendString(buffer);
        if(i>100)
        {
            // USART_SendString("warning!\r\n");
            break;
        }
    }

    //清除触发转换状态
    DL_ADC12_stopConversion(ADC1_INST);
    //失能ADC转换
    DL_ADC12_disableConversions(ADC1_INST);

    //获取数据
    gAdcResult = DL_ADC12_getMemResult(ADC1_INST, ADC1_ADCMEM_0 );

    return gAdcResult;
}