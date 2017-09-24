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

ADC_ChannelConfTypeDef adc_ch_conf;
ADC_HandleTypeDef adc_handle;

typedef struct {
	int16_t sensor_min_value;
	int16_t sensor_max_value;
	int16_t sensor_mid_value;
} config_sensor_t;

config_sensor_t sensor1_config;
config_sensor_t sensor2_config;
config_sensor_t sensor3_config;
config_sensor_t sensor4_config;
config_sensor_t sensor5_config;
config_sensor_t sensor6_config;
config_sensor_t sensor7_config;
config_sensor_t sensor8_config;
config_sensor_t sensor9_config;

int8_t system_init();

#endif /* __SYSTEM_INIT_H */

