/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     80000000



/* Defines for BUZZER */
#define BUZZER_INST                                                        TIMA0
#define BUZZER_INST_IRQHandler                                  TIMA0_IRQHandler
#define BUZZER_INST_INT_IRQN                                    (TIMA0_INT_IRQn)
#define BUZZER_INST_CLK_FREQ                                             2500000
/* GPIO defines for channel 3 */
#define GPIO_BUZZER_C3_PORT                                                GPIOB
#define GPIO_BUZZER_C3_PIN                                        DL_GPIO_PIN_24
#define GPIO_BUZZER_C3_IOMUX                                     (IOMUX_PINCM52)
#define GPIO_BUZZER_C3_IOMUX_FUNC                    IOMUX_PINCM52_PF_TIMA0_CCP3
#define GPIO_BUZZER_C3_IDX                                   DL_TIMER_CC_3_INDEX

/* Defines for PWM_THETA */
#define PWM_THETA_INST                                                     TIMG7
#define PWM_THETA_INST_IRQHandler                               TIMG7_IRQHandler
#define PWM_THETA_INST_INT_IRQN                                 (TIMG7_INT_IRQn)
#define PWM_THETA_INST_CLK_FREQ                                           500000
/* GPIO defines for channel 0 */
#define GPIO_PWM_THETA_C0_PORT                                             GPIOA
#define GPIO_PWM_THETA_C0_PIN                                     DL_GPIO_PIN_17
#define GPIO_PWM_THETA_C0_IOMUX                                  (IOMUX_PINCM39)
#define GPIO_PWM_THETA_C0_IOMUX_FUNC                 IOMUX_PINCM39_PF_TIMG7_CCP0
#define GPIO_PWM_THETA_C0_IDX                                DL_TIMER_CC_0_INDEX

/* Defines for PWM_PHI */
#define PWM_PHI_INST                                                       TIMA1
#define PWM_PHI_INST_IRQHandler                                 TIMA1_IRQHandler
#define PWM_PHI_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define PWM_PHI_INST_CLK_FREQ                                             500000
/* GPIO defines for channel 1 */
#define GPIO_PWM_PHI_C1_PORT                                               GPIOA
#define GPIO_PWM_PHI_C1_PIN                                       DL_GPIO_PIN_16
#define GPIO_PWM_PHI_C1_IOMUX                                    (IOMUX_PINCM38)
#define GPIO_PWM_PHI_C1_IOMUX_FUNC                   IOMUX_PINCM38_PF_TIMA1_CCP1
#define GPIO_PWM_PHI_C1_IDX                                  DL_TIMER_CC_1_INDEX

/* Defines for PWM_MOTOR */
#define PWM_MOTOR_INST                                                     TIMG6
#define PWM_MOTOR_INST_IRQHandler                               TIMG6_IRQHandler
#define PWM_MOTOR_INST_INT_IRQN                                 (TIMG6_INT_IRQn)
#define PWM_MOTOR_INST_CLK_FREQ                                         10000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_MOTOR_C0_PORT                                             GPIOA
#define GPIO_PWM_MOTOR_C0_PIN                                     DL_GPIO_PIN_21
#define GPIO_PWM_MOTOR_C0_IOMUX                                  (IOMUX_PINCM46)
#define GPIO_PWM_MOTOR_C0_IOMUX_FUNC                 IOMUX_PINCM46_PF_TIMG6_CCP0
#define GPIO_PWM_MOTOR_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_MOTOR_C1_PORT                                             GPIOA
#define GPIO_PWM_MOTOR_C1_PIN                                     DL_GPIO_PIN_22
#define GPIO_PWM_MOTOR_C1_IOMUX                                  (IOMUX_PINCM47)
#define GPIO_PWM_MOTOR_C1_IOMUX_FUNC                 IOMUX_PINCM47_PF_TIMG6_CCP1
#define GPIO_PWM_MOTOR_C1_IDX                                DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (4999U)




/* Defines for Sensor */
#define Sensor_INST                                                         I2C1
#define Sensor_INST_IRQHandler                                   I2C1_IRQHandler
#define Sensor_INST_INT_IRQN                                       I2C1_INT_IRQn
#define Sensor_BUS_SPEED_HZ                                               400000
#define GPIO_Sensor_SDA_PORT                                               GPIOA
#define GPIO_Sensor_SDA_PIN                                       DL_GPIO_PIN_18
#define GPIO_Sensor_IOMUX_SDA                                    (IOMUX_PINCM40)
#define GPIO_Sensor_IOMUX_SDA_FUNC                     IOMUX_PINCM40_PF_I2C1_SDA
#define GPIO_Sensor_SCL_PORT                                               GPIOA
#define GPIO_Sensor_SCL_PIN                                       DL_GPIO_PIN_29
#define GPIO_Sensor_IOMUX_SCL                                     (IOMUX_PINCM4)
#define GPIO_Sensor_IOMUX_SCL_FUNC                      IOMUX_PINCM4_PF_I2C1_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                            4000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_4_MHZ_115200_BAUD                                        (2)
#define UART_0_FBRD_4_MHZ_115200_BAUD                                       (11)
/* Defines for MOTOR */
#define MOTOR_INST                                                         UART1
#define MOTOR_INST_FREQUENCY                                             4000000
#define MOTOR_INST_IRQHandler                                   UART1_IRQHandler
#define MOTOR_INST_INT_IRQN                                       UART1_INT_IRQn
#define GPIO_MOTOR_RX_PORT                                                 GPIOB
#define GPIO_MOTOR_TX_PORT                                                 GPIOB
#define GPIO_MOTOR_RX_PIN                                          DL_GPIO_PIN_7
#define GPIO_MOTOR_TX_PIN                                          DL_GPIO_PIN_6
#define GPIO_MOTOR_IOMUX_RX                                      (IOMUX_PINCM24)
#define GPIO_MOTOR_IOMUX_TX                                      (IOMUX_PINCM23)
#define GPIO_MOTOR_IOMUX_RX_FUNC                       IOMUX_PINCM24_PF_UART1_RX
#define GPIO_MOTOR_IOMUX_TX_FUNC                       IOMUX_PINCM23_PF_UART1_TX
#define MOTOR_BAUD_RATE                                                 (115200)
#define MOTOR_IBRD_4_MHZ_115200_BAUD                                         (2)
#define MOTOR_FBRD_4_MHZ_115200_BAUD                                        (11)





/* Defines for ADC1 */
#define ADC1_INST                                                           ADC0
#define ADC1_INST_IRQHandler                                     ADC0_IRQHandler
#define ADC1_INST_INT_IRQN                                       (ADC0_INT_IRQn)
#define ADC1_ADCMEM_0                                         DL_ADC12_MEM_IDX_0
#define ADC1_ADCMEM_0_REF                        DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC1_ADCMEM_0_REF_VOLTAGE_V                                          3.3
#define GPIO_ADC1_C0_PORT                                                  GPIOA
#define GPIO_ADC1_C0_PIN                                          DL_GPIO_PIN_27



/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for D1: GPIOB.2 with pinCMx 15 on package pin 50 */
#define LED_D1_PIN                                               (DL_GPIO_PIN_2)
#define LED_D1_IOMUX                                             (IOMUX_PINCM15)
/* Defines for D2: GPIOB.3 with pinCMx 16 on package pin 51 */
#define LED_D2_PIN                                               (DL_GPIO_PIN_3)
#define LED_D2_IOMUX                                             (IOMUX_PINCM16)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOA)

/* Defines for K1: GPIOA.2 with pinCMx 7 on package pin 42 */
#define KEY_K1_PIN                                               (DL_GPIO_PIN_2)
#define KEY_K1_IOMUX                                              (IOMUX_PINCM7)
/* Defines for K4: GPIOA.23 with pinCMx 53 on package pin 24 */
#define KEY_K4_PIN                                              (DL_GPIO_PIN_23)
#define KEY_K4_IOMUX                                             (IOMUX_PINCM53)
/* Port definition for Pin Group MPU6050 */
#define MPU6050_PORT                                                     (GPIOA)

/* Defines for SCL: GPIOA.1 with pinCMx 2 on package pin 34 */
#define MPU6050_SCL_PIN                                          (DL_GPIO_PIN_1)
#define MPU6050_SCL_IOMUX                                         (IOMUX_PINCM2)
/* Defines for SDA: GPIOA.0 with pinCMx 1 on package pin 33 */
#define MPU6050_SDA_PIN                                          (DL_GPIO_PIN_0)
#define MPU6050_SDA_IOMUX                                         (IOMUX_PINCM1)
/* Port definition for Pin Group TB6612 */
#define TB6612_PORT                                                      (GPIOB)

/* Defines for AIN1: GPIOB.8 with pinCMx 25 on package pin 60 */
#define TB6612_AIN1_PIN                                          (DL_GPIO_PIN_8)
#define TB6612_AIN1_IOMUX                                        (IOMUX_PINCM25)
/* Defines for AIN2: GPIOB.9 with pinCMx 26 on package pin 61 */
#define TB6612_AIN2_PIN                                          (DL_GPIO_PIN_9)
#define TB6612_AIN2_IOMUX                                        (IOMUX_PINCM26)
/* Defines for BIN1: GPIOB.18 with pinCMx 44 on package pin 15 */
#define TB6612_BIN1_PIN                                         (DL_GPIO_PIN_18)
#define TB6612_BIN1_IOMUX                                        (IOMUX_PINCM44)
/* Defines for BIN2: GPIOB.19 with pinCMx 45 on package pin 16 */
#define TB6612_BIN2_PIN                                         (DL_GPIO_PIN_19)
#define TB6612_BIN2_IOMUX                                        (IOMUX_PINCM45)
/* Port definition for Pin Group GPIO_ENCODER */
#define GPIO_ENCODER_PORT                                                (GPIOA)

/* Defines for ENCODER1B: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GPIO_ENCODER_ENCODER1B_PIN                              (DL_GPIO_PIN_14)
#define GPIO_ENCODER_ENCODER1B_IOMUX                             (IOMUX_PINCM36)
/* Defines for ENCODER2B: GPIOA.15 with pinCMx 37 on package pin 8 */
#define GPIO_ENCODER_ENCODER2B_PIN                              (DL_GPIO_PIN_15)
#define GPIO_ENCODER_ENCODER2B_IOMUX                             (IOMUX_PINCM37)
/* Port definition for Pin Group Gray_Address */
#define Gray_Address_PORT                                                (GPIOA)

/* Defines for PIN_0: GPIOA.25 with pinCMx 55 on package pin 26 */
#define Gray_Address_PIN_0_PIN                                  (DL_GPIO_PIN_25)
#define Gray_Address_PIN_0_IOMUX                                 (IOMUX_PINCM55)
/* Defines for PIN_1: GPIOA.13 with pinCMx 35 on package pin 6 */
#define Gray_Address_PIN_1_PIN                                  (DL_GPIO_PIN_13)
#define Gray_Address_PIN_1_IOMUX                                 (IOMUX_PINCM35)
/* Defines for PIN_2: GPIOA.12 with pinCMx 34 on package pin 5 */
#define Gray_Address_PIN_2_PIN                                  (DL_GPIO_PIN_12)
#define Gray_Address_PIN_2_IOMUX                                 (IOMUX_PINCM34)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_BUZZER_init(void);
void SYSCFG_DL_PWM_THETA_init(void);
void SYSCFG_DL_PWM_PHI_init(void);
void SYSCFG_DL_PWM_MOTOR_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_Sensor_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_MOTOR_init(void);
void SYSCFG_DL_ADC1_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
