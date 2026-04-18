#include "bsp_motor_usart.h"
#include "servo_usart.h"

/************************************************
�������� �� Send_Motor_U8		Function name: Send_Motor_U8
��    �� �� USART1����һ���ַ�	Function: USART1 sends a character
��    �� �� Data --- ����		Parameter: Data --- data
�� �� ֵ �� ��					Return value: None
*************************************************/
void Send_Motor_U8(uint8_t Data)
{
	while( DL_UART_isBusy(MOTOR_INST) == true );
	DL_UART_Main_transmitData(MOTOR_INST, Data);
}

/************************************************
�������� �� Send_Motor_ArrayU8	Function name: Send_Motor_ArrayU8
��    �� �� ����1����N���ַ�		Function: Serial port 1 sends N characters
��    �� �� pData ---- �ַ���	Parameter: pData ---- string
            Length --- ����		Length --- length
�� �� ֵ �� ��					Return value: None
*************************************************/
void Send_Motor_ArrayU8(uint8_t *pData, uint16_t Length)
{
	while (Length--)
	{
		Send_Motor_U8(*pData);
		pData++;
	}
}


/*  �����жϽ��մ��� */
/* Serial port interrupt reception processing */
void Motor_INST_IRQHandler(void)
{
	uint8_t Rx2_Temp = 0;
	
	switch( DL_UART_getPendingInterrupt(MOTOR_INST) )
	{
		case DL_UART_IIDX_RX://����ǽ����ж�	If it is a receive interrupt
			// ���շ��͹��������ݱ���	Receive and save the data sent
			Rx2_Temp = DL_UART_Main_receiveData(MOTOR_INST);
			//����	deal with
			Deal_Control_Rxtemp(Rx2_Temp);
			break;
		
		default://�����Ĵ����ж�	Other serial port interrupts
			break;
	}	
	

}
