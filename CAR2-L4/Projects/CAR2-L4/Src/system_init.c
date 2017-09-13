
#include "system_init.h"

GPIO_InitTypeDef GPIO_InitDef;
TIM_HandleTypeDef proxi_pwm_handle;
TIM_OC_InitTypeDef proxi_pwm_oc_init;
ADC_HandleTypeDef adc_handle;
ADC_ChannelConfTypeDef adc_ch_conf;
ADC_HandleTypeDef adc_12b_handle;

int8_t pin_init();
int8_t portA_init();
int8_t portB_init();
int8_t portC_init();
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

	// Initialize pins D0, D1 and D7 (PA1, PA0, PA4) as ADC input
	GPIO_InitDef.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4;
	GPIO_InitDef.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	GPIO_InitDef.Pull = GPIO_NOPULL;

#ifdef DEBUG_MODE
	printf("Digital ADC pins init done.\n");
#endif

	//init D4 (PA3) pin EXTI mode
	GPIO_InitDef.Pin = GPIO_PIN_3;
	GPIO_InitDef.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitDef.Pull = GPIO_PULLUP;

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


int8_t portC_init()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitDef.Speed = GPIO_SPEED_FAST;

	// Initialize pins A0 - A5 (PC5 - PC0) as ADC input
	GPIO_InitDef.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitDef.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	GPIO_InitDef.Pull = GPIO_NOPULL;

#ifdef DEBUG_MODE
	printf("Analog ADC pins init done.\n");
#endif

	HAL_GPIO_Init(GPIOC, &GPIO_InitDef);

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


void adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();

	adc_handle.State = HAL_ADC_STATE_RESET;
	adc_handle.Instance = ADC1;
	adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; //
	adc_handle.Init.Resolution = ADC_RESOLUTION_8B;
	adc_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc_handle.Init.DMAContinuousRequests = DISABLE;
	adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle.Init.ContinuousConvMode = DISABLE;
	adc_handle.Init.DiscontinuousConvMode = DISABLE;
	adc_handle.Init.ScanConvMode = DISABLE;
	adc_handle.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&adc_handle);

	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
}


void adc_12b_init()		// ADC2 channel 7 on pin D10 (PA2)
{
	adc_12b_handle.State = HAL_ADC_STATE_RESET;
	adc_12b_handle.Instance = ADC2;
	adc_12b_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_12b_handle.Init.Resolution = ADC_RESOLUTION_12B;
	adc_12b_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc_12b_handle.Init.DMAContinuousRequests = DISABLE;
	adc_12b_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_12b_handle.Init.ContinuousConvMode = DISABLE;
	adc_12b_handle.Init.DiscontinuousConvMode = DISABLE;
	adc_12b_handle.Init.ScanConvMode = DISABLE;
	HAL_ADC_Init(&adc_12b_handle);

	adc_ch_conf.Channel = ADC_CHANNEL_7;
	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	HAL_ADC_ConfigChannel(&adc_12b_handle, &adc_ch_conf);
}


void a0_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_14;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a1_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_13;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a2_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_4;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a3_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_3;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a4_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_2;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a5_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_1;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d0_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_6;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d1_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_5;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d7_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_9;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}
