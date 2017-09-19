/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H

#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "uart.h"
#include <string.h>

TIM_HandleTypeDef servo_pwm_handle;
TIM_HandleTypeDef motor_pwm_handle;
TIM_OC_InitTypeDef servo_pwm_oc_init;
TIM_OC_InitTypeDef motor_pwm_oc_init;
TIM_HandleTypeDef ic_handle;

ADC_HandleTypeDef sensor1_handle;
ADC_HandleTypeDef sensor2_handle;
ADC_HandleTypeDef sensor3_handle;
ADC_HandleTypeDef sensor4_handle;
ADC_HandleTypeDef sensor5_handle;
ADC_HandleTypeDef sensor6_handle;
ADC_HandleTypeDef sensor7_handle;
ADC_HandleTypeDef sensor8_handle;
ADC_HandleTypeDef sensor9_handle;

int8_t system_init();

#endif /* __SYSTEM_INIT_H */

