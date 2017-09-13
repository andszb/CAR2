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

int8_t system_init();
void a0_adc_init();
void a1_adc_init();
void a2_adc_init();
void a3_adc_init();
void a4_adc_init();
void a5_adc_init();
void d0_adc_init();
void d1_adc_init();
void d7_adc_init();

#endif /* __SYSTEM_INIT_H */

