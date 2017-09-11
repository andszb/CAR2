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
	portB_init();
	portD_init();
	return 0;
}

// init portA pins
int8_t portA_init()
{
	GPIO_InitTypeDef GPIO_Init;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//init D4 (PA3) pin EXTI mode
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

#ifdef DEBUG_MODE
	printf("Proxim EXTI pin init done.\n");
#endif

	//Initialize D11 and D12 (PA6 and PA7) as LED output
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
#ifdef DEBUG_MODE
	printf("Proxim LED init done.\n");
#endif
	return 0;
}

// init portB pins
int8_t portB_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//init D3 (PB0) as trigger for proximity sensors
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#ifdef DEBUG_MODE
	printf("Proxim trigger1 init done.\n");
#endif



	return 0;
}

int8_t portD_init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	//init D2 (PD14) as trigger for proximity sensors
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_14;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOD, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
#ifdef DEBUG_MODE
	printf("Proxim trigger2 init done.\n");
#endif
	return 0;
}
