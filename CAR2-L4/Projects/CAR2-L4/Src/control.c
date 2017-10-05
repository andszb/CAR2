#include "control.h"
#include "system_init.h"
#include "servo_control.h"
#include "motor_control.h"
#include "proximity_driver.h"
#include "adc_driver.h"

//#define DEBUG_MODE

uint8_t button_pressed = 0;
uint32_t measured_distance = 0;

void control_thread()
{
//	motor_pwm_set_duty(100);
//	HAL_Delay(20);
//	motor_pwm_set_duty(15);
//	HAL_Delay(20);
//	required_rpm = 250;

	while (1) {
		if (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET) {
			if (!button_pressed) {
				required_rpm = 100;
				button_pressed = 1;
//				HAL_Delay(100);
			} else {
				required_rpm = 0;
				button_pressed = 0;
//				HAL_Delay(100);
			}
		}
//		check for any object
		measured_distance = read_proximity_data();
		process_proximity(measured_distance);
#ifdef DEBUG_MODE
		printf("\n\ndistance: %lu\n", measured_distance);
#endif
// 		determine line position
		int8_t measured_line_position = handle_line_position();
//		turn servo based on line position
		turn_servo(measured_line_position);
#ifdef DEBUG_MODE
		printf("\n\nline position: %d\n", measured_line_position);
#endif
//		determine required rpm
		float dc = pi_control();
//		uint8_t idc = (uint8_t)dc;
//		printf("PI: %d\n", idc);
		motor_pwm_set_duty(dc);
		HAL_Delay(20);
//		osDelay(1000);
	}

	terminate_thread();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM5)
    	ovf_cntr++;
    else
    	cm_cntr++;
}

void terminate_thread()
{
	while (1)
		osThreadTerminate(NULL);
}

