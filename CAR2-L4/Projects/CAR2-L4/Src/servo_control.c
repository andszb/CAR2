#include "system_init.h"
#include "servo_control.h"
#include "adc_driver.h"

#define LINE_DETECT_DELAY 10


const float ctrler_out_min = -36.0;
const float ctrler_out_max = 36.0;
float p_value = 1.0;
float d_value = 1.0;
int16_t angle_error = 0;
int16_t prev_error = 0;
int16_t difference = 0;
float ctrler_out = 0.0;
int8_t last_line_position = 0;

void servo_pwm_set_duty(float duty);
/*void set_servo_angle()
{
 *
 *	5% duty cycle is the leftmost position of the steering, 10% is the rightmost.
 *	If we say leftmost is -36 degrees, rightmost is 36,
 *	then 1 degree equals to (5 / 72) % in duty cycle.
 *	7.5 % is 0 degrees.
 *
	float angle = pd_control();
	float duty = 7.5 + ((5.0 / 72.0) * (float)angle);
	servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
	printf("angle: %.2f, duty: %.2f; ", angle, duty);
#endif
}
*/

void servo_pwm_set_duty(float duty)
{
	uint32_t pulse = servo_pwm_handle.Init.Period * (duty / 100.0);
	servo_pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&servo_pwm_handle, &servo_pwm_oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&servo_pwm_handle, TIM_CHANNEL_1);
	return;
}

float pd_control(int8_t line_position_tmp)
{
	//convert line position into angle: from middle position one sensor left or right means 36/4 = 9 degree error
	angle_error = line_position_tmp * 9;
	difference = angle_error - prev_error;
	ctrler_out = p_value * (float)angle_error + d_value * (float)difference;
	if (ctrler_out < ctrler_out_min) {
		ctrler_out = ctrler_out_min;
	}
	else if (ctrler_out > ctrler_out_max) {
		ctrler_out = ctrler_out_max;
	}
	prev_error = angle_error;
#ifdef DEBUG_MODE
	printf("pd ctrler out angle: %.2f\n; ", ctrler_out);
#endif
	return ctrler_out;
}

//turn servo based on line position
void turn_servo(int8_t line_position_tmp)
{
	float angle;
	float duty;
	if (no_line_flag == 0){
		//If line position is detected, turn servo
		angle = pd_control(line_position_tmp);
		duty = 7.5 + (5.0 / 72.0 * angle);
		servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %.2f, duty: %.2f; ", angle, duty);
#endif
		//save last line position
		last_line_position = line_position_tmp;

	} else if(no_line_flag == 1){
		//If no line is detected
		for (int8_t i = 0; i < LINE_DETECT_DELAY; i++){
			//and the last line position was sensor #1
			//keep turning 'LINE_DETECT_DELAY' times
			if (last_line_position == 4){
				angle = -36.0;
				duty = 7.5 + ((5.0 / 72.0) * angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %.2f, duty: %.2f; ", angle, duty);
#endif
			} else if (last_line_position == -4){
				//and the last line position was sensor #9
				//keep turning 'LINE_DETECT_DELAY' times
				angle = 36.0;
				duty = 7.5 + ((5.0 / 72.0) * angle);
#ifdef DEBUG_MODE
		printf("angle: %.2f, duty: %.2f; ", angle, duty);
#endif
				servo_pwm_set_duty(duty);
			}
		}
		//if there is still no line detected, try to turn to the opposite direction
		for (int8_t i = 0; i < LINE_DETECT_DELAY; i++){
			if (last_line_position == 4){
				angle = 36.0;
				duty = 7.5 + ((5.0 / 72.0) * angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %.2f, duty: %.2f; ", angle, duty);
#endif
			} else if (last_line_position == -4){
				angle = -36.0;
				duty = 7.5 + ((5.0 / 72.0) * angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %.2f, duty: %.2f; ", angle, duty);
#endif
			}
		}
		//if there is no line found, stop the car
	} else {
		//stop car!
	}
}
