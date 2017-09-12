#include "servo_control.h"

void set_servo_angle(int8_t angle);
void servo_pwm_set_duty(float duty);

void set_servo_angle(int8_t angle)
{
/*
 *	5% duty cycle is the leftmost position of the steering, 10% is the rightmost.
 *	If we say leftmost is -36 degrees, rightmost is 36,
 *	then 1 degree equals to (5 / 72) % in duty cycle.
 *	7.5 % is 0 degrees.
 */
	float duty = 7.5 + ((5.0 / 72.0) * (float)angle);
	servo_pwm_set_duty(duty);
}

void servo_pwm_set_duty(float duty)
{
	uint32_t pulse = servo_pwm_handle.Init.Period * (duty / 100.0);
	servo_pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&servo_pwm_handle, &servo_pwm_oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&servo_pwm_handle, TIM_CHANNEL_1);
	return;
}
