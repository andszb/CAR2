#include "motor_control.h"
#include "system_init.h"
#include <math.h> //for isnan, isinf

typedef struct {
	uint32_t ovf;
	uint32_t prev;
	uint32_t last;
} input_capture_data_t;

input_capture_data_t ic_cntr = {0, 0, 0};
uint32_t ovf_cntr = 0;
float prev_rpm_value = 0;

const float m_ctrler_out_min = 0;
const float m_ctrler_out_max = 100;

float motor_p_value = 0.1;
float i_value = 0.1;
int16_t motor_error = 0;
int16_t integral = 0;
int16_t required_rpm = 0;
int16_t measured_rpm = 0;
float m_ctrler_out = 0.0;
float duty;

void print_float(float value, int decimal_digits);
float get_rpm();


void print_float(float value, int decimal_digits)
{
	int i = 1;
	int int_part, fract_part;
	for ( ; decimal_digits != 0; i *= 10, decimal_digits--);
	int_part = (int)value;
	fract_part = (int)((value - (float)(int)value) * i);
	if (fract_part < 0)
		fract_part *= -1;
	printf("%d.%d", int_part, fract_part);
}


void set_direction(int8_t dir)
{
	if (dir == 1) {				// set forward
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	} else if (dir == -1) {		// set reverse
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	}
}


float pi_control()
{
	motor_error = required_rpm - (uint16_t)get_rpm();
	integral += motor_error;
	m_ctrler_out = motor_p_value * (float)motor_error + i_value * (float)integral;
	if (m_ctrler_out < m_ctrler_out_min) {
		m_ctrler_out = m_ctrler_out_min;
		integral -= motor_error;
	}
	else if (m_ctrler_out > m_ctrler_out_max) {
		m_ctrler_out = m_ctrler_out_max;
		integral -= motor_error;
	}
	return m_ctrler_out;
}


void stop_drive()
{
	HAL_TIM_PWM_Stop(&motor_pwm_handle, TIM_CHANNEL_1);
}


void disable_drive()
{
	stop_drive();
	HAL_TIM_PWM_DeInit(&motor_pwm_handle);
	// Disable output pin
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
}


void go()
{
	for (uint8_t i = 25; i > 15; i--) {
		duty = i;
		motor_pwm_set_duty(duty);
		osDelay(30);
	}
}


void accelerate()
{
	duty *= 1.2;
	motor_pwm_set_duty(duty);
}


void decelerate()
{
	duty *= 0.8;
	motor_pwm_set_duty(duty);
}


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	ovf_cntr++;
//}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	ic_cntr.prev = ic_cntr.last;
	ic_cntr.last = TIM2 -> CCR1;
	ic_cntr.ovf = ovf_cntr;
	ovf_cntr = 0;
}


float get_freq()
{
	HAL_NVIC_DisableIRQ(TIM5_IRQn);
	input_capture_data_t snapshot = ic_cntr;
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

	float steps = (float)snapshot.ovf * ic_handle.Init.Period + snapshot.last - snapshot.prev;
	float tim5_clk_freq = (float)SystemCoreClock / 2 / (ic_handle.Init.Prescaler + 1); // Because clock division is 1x, so only sysclock matters
	float tim5_clk_period = 1 / tim5_clk_freq;
	float signal_period = steps * tim5_clk_period;
	float signal_freq = 1 / signal_period;

	if (isnan(signal_freq) || isinf(signal_freq))
		return -1;
	else
		return signal_freq;
}


float get_rpm()
{
	float rpm = get_freq() * 60;
	if (rpm < 0) {
		return prev_rpm_value;
	} else {
		prev_rpm_value = rpm;
		return rpm;
	}
}


void motor_pwm_set_duty(float duty)
{
	uint32_t pulse = motor_pwm_handle.Init.Period * (duty / 100.0);
	motor_pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&motor_pwm_handle, &motor_pwm_oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&motor_pwm_handle, TIM_CHANNEL_1);
	return;
}
