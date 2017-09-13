#include "servo_control.h"

const float ctrler_out_min = -36.0;
const float ctrler_out_max = 36.0;
float p_value = 0.1;
float d_value = 0.1;
int16_t error = 0;
int16_t sensor_number = 0;  //temp variable for testing
int16_t prev_error = 0;
int16_t difference = 0;
float ctrler_out = 0.0;

void servo_pwm_set_duty(float duty);

void set_servo_angle(float angle)
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

float pd_control()
{
	error = sensor_number;		// not yet defined
	difference = error - prev_error;
	ctrler_out = p_value * (float)error + d_value * (float)difference;
	if (ctrler_out < ctrler_out_min) {
		ctrler_out = ctrler_out_min;
	}
	else if (ctrler_out > ctrler_out_max) {
		ctrler_out = ctrler_out_max;
	}
	prev_error = error;

	return ctrler_out;
}
