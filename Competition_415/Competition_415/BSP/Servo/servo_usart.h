#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define SERVO_INST MOTOR_INST
#define SERVO_INST_FREQUENCY MOTOR_INST_FREQUENCY
#define SERVO_INST_IRQHandler MOTOR_INST_IRQHandler
#define SERVO_INST_INT_IRQN MOTOR_INST_INT_IRQN
#define GPIO_SERVO_RX_PORT GPIO_MOTOR_RX_PORT
#define GPIO_SERVO_TX_PORT GPIO_MOTOR_TX_PORT
#define GPIO_SERVO_RX_PIN GPIO_MOTOR_RX_PIN
#define GPIO_SERVO_TX_PIN GPIO_MOTOR_TX_PIN
#define GPIO_SERVO_IOMUX_RX GPIO_MOTOR_IOMUX_RX
#define GPIO_SERVO_IOMUX_TX GPIO_MOTOR_IOMUX_TX
#define GPIO_SERVO_IOMUX_RX_FUNC GPIO_MOTOR_IOMUX_RX_FUNC
#define GPIO_SERVO_IOMUX_TX_FUNC GPIO_MOTOR_IOMUX_TX_FUNC
#define SERVO_BAUD_RATE MOTOR_BAUD_RATE
#define SERVO_IBRD_4_MHZ_115200_BAUD MOTOR_IBRD_4_MHZ_115200_BAUD
#define SERVO_FBRD_4_MHZ_115200_BAUD MOTOR_FBRD_4_MHZ_115200_BAUD

#define CENTER_X 160
#define CENTER_Y 120

void splitString(char* mystrArray[],char *str, const char *delimiter);
void Deal_Control_Rxtemp(uint8_t rxtemp);
void Deal_data_real(void);

extern int Pos_Now[2];