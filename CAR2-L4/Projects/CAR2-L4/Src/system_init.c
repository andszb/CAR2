
#include "system_init.h"

GPIO_InitTypeDef GPIO_InitDef;
TIM_HandleTypeDef proxi_pwm_handle;
TIM_OC_InitTypeDef proxi_pwm_oc_init;

int8_t pin_init();
int8_t portA_init();
int8_t portB_init();
int8_t portD_init();
int8_t servo_pwm_init();
int8_t motor_pwm_init();


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


// init port A pins
int8_t portA_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitDef.Speed = GPIO_SPEED_FAST;

	//init D4 (PA3) pin EXTI mode
	GPIO_InitDef.Pin = GPIO_PIN_3;
	GPIO_InitDef.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitDef.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Proxim EXTI pin init done.\n");
#endif

	// Initialize pin D9 (PA15) as PWM (TIM2) output
	GPIO_InitDef.Pin = GPIO_PIN_15;
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Alternate = GPIO_AF1_TIM2;

#ifdef DEBUG_MODE
	printf("Motor PWM pin init done.\n");
#endif

	//Initialize D11 and D12 (PA6 and PA7) as LED output
	GPIO_InitDef.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;

#ifdef DEBUG_MODE
	printf("Proxim LED init done.\n");
#endif

	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	return 0;
}


// init port B pins
int8_t portB_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitDef.Speed = GPIO_SPEED_FAST;

	//init D3 (PB0) as trigger for proximity sensors
	GPIO_InitDef.Pin = GPIO_PIN_0;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_PULLUP;

#ifdef DEBUG_MODE
	printf("Proxim trigger1 init done.\n");
#endif

	// Initialize pin D5 (PB4) as PWM (TIM3) output
	GPIO_InitDef.Pin = GPIO_PIN_4;
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Alternate = GPIO_AF2_TIM3;

#ifdef DEBUG_MODE
	printf("Servo PWM pin init done.\n");
#endif

	// Initialize pin D6 and D8 (PB1 and PB2) as output for motor direction control
	GPIO_InitDef.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;

#ifdef DEBUG_MODE
	printf("Motor direction pin init done.\n");
#endif

	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

	return 0;
}


int8_t portD_init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitDef.Speed = GPIO_SPEED_FAST;

	//init D2 (PD14) as trigger for proximity sensors
	GPIO_InitDef.Pin = GPIO_PIN_14;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_PULLUP;

#ifdef DEBUG_MODE
	printf("Proxim trigger2 init done.\n");
#endif

	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

	return 0;
}


int8_t servo_pwm_init()
{
	// TIM3 init as PWM, 50 Hz
	__HAL_RCC_TIM3_CLK_ENABLE();
	servo_pwm_handle.Instance = TIM3;
	servo_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	servo_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	servo_pwm_handle.Init.Period = 31380;
	servo_pwm_handle.Init.Prescaler = 50;
	if (HAL_TIM_PWM_Init(&servo_pwm_handle) != HAL_OK) {
		return -1;
	}

	servo_pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	servo_pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	servo_pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	servo_pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	servo_pwm_oc_init.Pulse = 2354;
	HAL_TIM_PWM_ConfigChannel(&servo_pwm_handle, &servo_pwm_oc_init, TIM_CHANNEL_1);

	return 0;
}


int8_t motor_pwm_init()
{
	// TIM2 init as PWM, 1 kHz
	__HAL_RCC_TIM2_CLK_ENABLE();
	motor_pwm_handle.Instance = TIM2;
	motor_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	motor_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	motor_pwm_handle.Init.Period = 16000;
	motor_pwm_handle.Init.Prescaler = 4;
	if (HAL_TIM_PWM_Init(&motor_pwm_handle) != HAL_OK) {
		return -1;
	}

	motor_pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	motor_pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	motor_pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	motor_pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	motor_pwm_oc_init.Pulse = 4000;
	HAL_TIM_PWM_ConfigChannel(&motor_pwm_handle, &motor_pwm_oc_init, TIM_CHANNEL_1);

	return 0;
}
