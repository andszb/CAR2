/*
 * system_init.c
 *
 *  Created on: 2017. szept. 11.
 *      Author: AND
 */
#include "system_init.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

GPIO_InitTypeDef GPIO_InitDef;
TIM_HandleTypeDef servo_pwm_handle;
TIM_HandleTypeDef proxi_pwm_handle;
TIM_OC_InitTypeDef servo_pwm_oc_init;
TIM_OC_InitTypeDef motor_pwm_oc_init;
TIM_OC_InitTypeDef proxi_pwm_oc_init;

int8_t system_init();
int8_t pin_init();
int8_t portA_init();
int8_t portB_init();

//call init functions
int8_t system_init()
{
	pin_init();


	return 0;
}

//call port init functions
int8_t pin_init()
{
	portA_init();
	return 0;
}

// init port A pins
int8_t portA_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Initialize pin D9 (PA15) as PWM (TIM2) output
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitDef.Pin = GPIO_PIN_15;
	GPIO_InitDef.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

	return 0;
}

// init port B pins
int8_t portB_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Initialize pin D5 (PB4) as PWM (TIM3) output
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitDef.Pin = GPIO_PIN_4;
	GPIO_InitDef.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

	return 0;
}
