#include "motor_control.h"

float ctrler_out_min = 0;
float ctrler_out_max = 100;

float p_value = 0.1;
float i_value = 0.1;
int16_t error = 0;
int16_t integral = 0;
int16_t required_rpm = 0;
int16_t measured_rpm = 0;
float ctrler_out = 0.0;
float duty;

void print_float(float value, int decimal_digits);


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
	error = required_rpm - measured_rpm;
	integral += error;
	ctrler_out = p_value * (float)error + i_value * (float)integral;
	if (ctrler_out < ctrler_out_min) {
		ctrler_out = ctrler_out_min;
		integral -= error;
	}
	else if (ctrler_out > ctrler_out_max) {
		ctrler_out = ctrler_out_max;
		integral -= error;
	}
	return ctrler_out;
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


void motor_pwm_set_duty(float duty)
{
	uint32_t pulse = motor_pwm_handle.Init.Period * (duty / 100.0);
	motor_pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&motor_pwm_handle, &motor_pwm_oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&motor_pwm_handle, TIM_CHANNEL_1);
	return;
}
