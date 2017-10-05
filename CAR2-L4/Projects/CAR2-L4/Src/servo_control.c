#include "system_init.h"
#include "servo_control.h"
#include "adc_driver.h"

#define LINE_DETECT_DELAY 100
#define MAX_ANGLE 36.0
#define MIN_ANGLE -36.0
#define DUTY_EXCHANGE_RATE 0.0694
#define DUTY_MID_POINT 7.5

/*
 * Damping ratio is less than one.
 * Fast response, but system will overshoot.
 */

float p_value = 986.96;
float d_value = 57.03;
int16_t angle_error = 0;
int16_t prev_error = 0;
int16_t difference = 0;
float ctrler_out = 0.0;
int8_t last_line_position = 0;

void servo_pwm_set_duty(float duty);
float calculate_duty(float angle_tmp);



float calculate_duty(float angle_tmp)
{
	float duty_tmp = (DUTY_EXCHANGE_RATE * angle_tmp) + DUTY_MID_POINT;

#ifdef DEBUG_MODE
	printf("angle: %f, duty: %f; ", angle_tmp, duty);
#endif
	return duty_tmp;
}


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
	if (ctrler_out < MIN_ANGLE) {
		ctrler_out = MIN_ANGLE;
	}
	else if (ctrler_out > MAX_ANGLE) {
		ctrler_out = MAX_ANGLE;
	}
	prev_error = angle_error;
#ifdef DEBUG_MODE
	printf("pd ctrler out angle: %f\n; ", ctrler_out);
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
		duty = calculate_duty(angle);
		servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %f, duty: %f; ", angle, duty);
#endif
		//save last line position
		last_line_position = line_position_tmp;

	} else if(no_line_flag == 1){
		//If no line is detected
		for (int8_t i = 0; i < LINE_DETECT_DELAY; i++){
			//and the last line position was sensor #1
			//keep turning 'LINE_DETECT_DELAY' times
			if (last_line_position == 4){
				angle = 36.0;
				duty = calculate_duty(angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %f, duty: %f; ", angle, duty);
#endif
			} else if (last_line_position == -4){
				//and the last line position was sensor #9
				//keep turning 'LINE_DETECT_DELAY' times
				angle = -36.0;
				duty = calculate_duty(angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %f, duty: %f; ", angle, duty);
#endif
			}
			//HAL_Delay(3);
		}
		//if there is still no line detected, try to turn to the opposite direction
		for (int8_t i = 0; i < LINE_DETECT_DELAY; i++){
			if (last_line_position == 4){
				angle = -36.0;
				duty = calculate_duty(angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %f, duty: %f; ", angle, duty);
#endif
			} else if (last_line_position == -4){
				angle = 36.0;
				duty = calculate_duty(angle);
				servo_pwm_set_duty(duty);
#ifdef DEBUG_MODE
		printf("angle: %f, duty: %f; ", angle, duty);
#endif
			}
			//HAL_Delay(3);
		}
		//if no line is found, stop the car
	} else {
		//stop car!
	}
}
