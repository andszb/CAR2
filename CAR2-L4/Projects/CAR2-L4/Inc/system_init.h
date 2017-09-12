/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H
#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"

TIM_HandleTypeDef servo_pwm_handle;
TIM_HandleTypeDef motor_pwm_handle;
TIM_OC_InitTypeDef servo_pwm_oc_init;
TIM_OC_InitTypeDef motor_pwm_oc_init;

int8_t systeminit();

#endif /* __SYSTEM_INIT_H */

