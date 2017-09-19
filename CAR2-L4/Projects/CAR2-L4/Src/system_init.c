
#include "system_init.h"

#define DEBUG_MODE

GPIO_InitTypeDef GPIO_InitDef;
TIM_HandleTypeDef proxi_pwm_handle;
TIM_HandleTypeDef proxim_timer_handle;
ADC_HandleTypeDef adc_handle;
//ADC_HandleTypeDef adc_12b_handle;
ADC_ChannelConfTypeDef adc_ch_conf;
TIM_OC_InitTypeDef proxi_pwm_oc_init;
TIM_IC_InitTypeDef rpm_ic_init;

int8_t pins_init();
int8_t timers_init();
int8_t interrupts_init();
int8_t portA_init();
int8_t portB_init();
int8_t portC_init();
int8_t portD_init();
int8_t servo_pwm_init();
int8_t motor_pwm_init();
int8_t proximity_timer_init();
int8_t rpm_measure_init();
static void EXTI3_IRQHandler_Config(void);
int8_t proximity_exti_init();
static void TIM5_IRQHandler_Config(void);
int8_t tim5_ic_it_init();



//call init functions
int8_t system_init()
{
	BSP_LED_Init(LED2);

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	uart_init();

	/* Output without printf, using HAL function*/
	char msg[] = "UART HAL Example\r\n";
	HAL_UART_Transmit(&uart_handle, (uint8_t *)msg, strlen(msg), 100);

	/* Output a message using printf function */
#ifdef DEBUG_MODE
	printf("UART Printf Example: retarget the C library printf function to the UART\r\n");
	printf("** Test finished successfully. ** \r\n");
#endif
	//call pin init functions
	pins_init();

	//call timers init functions
	timers_init();

	//call interrups init functions
	interrupts_init();

	return 0;
}


//call port init functions
int8_t pins_init()
{
	portA_init();
	portB_init();
	portC_init();
	portD_init();
	return 0;
}

//call timers init functions
int8_t timers_init()
{
	servo_pwm_init();
	motor_pwm_init();
	proximity_timer_init();
	rpm_measure_init();

	return 0;

}

int8_t interrupts_init()
{
	proximity_exti_init();
	tim5_ic_it_init();

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
	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Digital ADC pins init done.\n");
#endif

	//init D4 (PA3) pin as TIM5 IC input
	GPIO_InitDef.Pin = GPIO_PIN_3;
	GPIO_InitDef.Mode = GPIO_MODE_IT_RISING;	// maybe AF_OD?
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Alternate = GPIO_AF2_TIM5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("RPM IC pin init done.\n");
#endif

	//init D13 (PA5) pin EXTI mode
	GPIO_InitDef.Pin = GPIO_PIN_5;
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
	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Motor PWM pin init done.\n");
#endif

	//Initialize D11 and D12 (PA6 and PA7) as LED output
	GPIO_InitDef.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Proxim LED init done.\n");
#endif

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
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Proxim trigger1 init done.\n");
#endif

	// Initialize pin D5 (PB4) as PWM (TIM3) output
	GPIO_InitDef.Pin = GPIO_PIN_4;
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Servo PWM pin init done.\n");
#endif

	// Initialize pin D6 and D8 (PB1 and PB2) as output for motor direction control
	GPIO_InitDef.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Motor direction pin init done.\n");
#endif

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
	HAL_GPIO_Init(GPIOC, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Analog ADC pins init done.\n");
#endif


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
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);

#ifdef DEBUG_MODE
	printf("Proxim trigger2 init done.\n");
#endif

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
	if (HAL_TIM_PWM_Init(&servo_pwm_handle) != HAL_OK)
		return -1;

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
	if (HAL_TIM_PWM_Init(&motor_pwm_handle) != HAL_OK)
		return -1;

	motor_pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	motor_pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	motor_pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	motor_pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	motor_pwm_oc_init.Pulse = 4000;
	HAL_TIM_PWM_ConfigChannel(&motor_pwm_handle, &motor_pwm_oc_init, TIM_CHANNEL_1);

	return 0;
}


int8_t proximity_timer_init()
{
	//init TIM4_CH3 10 kHz x 0,58 = 1 period / 1 cm
	__HAL_RCC_TIM4_CLK_ENABLE();
	HAL_NVIC_SetPriority(TIM4_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);

	proxim_timer_handle.Instance = TIM4;
	proxim_timer_handle.State = HAL_TIM_STATE_RESET;
	proxim_timer_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_3;
	proxim_timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	proxim_timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	proxim_timer_handle.Init.Period = 928;
	proxim_timer_handle.Init.Prescaler = 4;
	if (HAL_TIM_Base_Init(&proxim_timer_handle) != HAL_OK)
		return -1;
	if (HAL_TIM_Base_Start_IT(&proxim_timer_handle) != HAL_OK)
		return -1;

#ifdef DEBUG_MODE
	printf("TIM4 init done.\n");
#endif

	return 0;
}


int8_t rpm_measure_init()
{
	__HAL_RCC_TIM5_CLK_ENABLE();

	ic_handle.Instance = TIM5;
	ic_handle.State = HAL_TIM_STATE_RESET;
	ic_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_4;
	ic_handle.Init.RepetitionCounter = 0xFF;
	ic_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	ic_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	ic_handle.Init.Period = 0xFFFF;
	ic_handle.Init.Prescaler = 0;
	HAL_TIM_IC_Init(&ic_handle);

	rpm_ic_init.ICFilter = 0;
	rpm_ic_init.ICPolarity = TIM_ICPOLARITY_RISING;
	rpm_ic_init.ICPrescaler = TIM_ICPSC_DIV1;
	rpm_ic_init.ICSelection = TIM_ICSELECTION_DIRECTTI;
	HAL_TIM_IC_ConfigChannel(&ic_handle, &rpm_ic_init, TIM_CHANNEL_4);

	if (HAL_TIM_Base_Start_IT(&ic_handle) != HAL_OK)
		return -1;
	if (HAL_TIM_IC_Start_IT(&ic_handle, TIM_CHANNEL_4) != HAL_OK)
		return -1;

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


//void adc_12b_init()		// ADC2 channel 7 on pin D10 (PA2)
//{
//	adc_12b_handle.State = HAL_ADC_STATE_RESET;
//	adc_12b_handle.Instance = ADC2;
//	adc_12b_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
//	adc_12b_handle.Init.Resolution = ADC_RESOLUTION_12B;
//	adc_12b_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//	adc_12b_handle.Init.DMAContinuousRequests = DISABLE;
//	adc_12b_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	adc_12b_handle.Init.ContinuousConvMode = DISABLE;
//	adc_12b_handle.Init.DiscontinuousConvMode = DISABLE;
//	adc_12b_handle.Init.ScanConvMode = DISABLE;
//	HAL_ADC_Init(&adc_12b_handle);
//
//	adc_ch_conf.Channel = ADC_CHANNEL_7;
//	adc_ch_conf.Offset = 0;
//	adc_ch_conf.Rank = 1;
//	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
//	HAL_ADC_ConfigChannel(&adc_12b_handle, &adc_ch_conf);
//}


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


static void EXTI3_IRQHandler_Config(void)
{
	/* Enable and set EXTI lines 3 Interrupt to priority 3*/
	HAL_NVIC_SetPriority(EXTI3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}


int8_t proximity_exti_init()
{
	//init D4 (PA3) EXTI mode
	EXTI3_IRQHandler_Config();
#ifdef DEBUG_MODE
	printf("Proxim sensor init done.\n");
#endif
	return 0;
}


static void TIM5_IRQHandler_Config(void)
{
	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}


int8_t tim5_ic_it_init()
{
	TIM5_IRQHandler_Config();
#ifdef DEBUG_MODE
	printf("TIM5 IC interrupt init done.\n");
#endif
	return 0;
}



