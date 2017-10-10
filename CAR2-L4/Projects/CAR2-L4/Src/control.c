#include "control.h"
#include "system_init.h"
#include "servo_control.h"
#include "motor_control.h"
#include "proximity_driver.h"
#include "wifi_functions.h"
//#include "adc_driver.h"

#define DEBUG_MODE

//uint8_t button_pressed = 0;

void control_thread()
{
	while (1) {
		if (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET) {
//			if (!button_pressed) {
				go();
//				motor_pwm_set_duty(17);
//				button_pressed = 1;
//				HAL_Delay(100);
//			} else {
//				stop();
//				button_pressed = 0;
//				HAL_Delay(100);
//			}
		}
		// check for any object
		uint32_t measured_distance = read_proximity_data();
		process_proximity(measured_distance);

#ifdef DEBUG_MODE
		printf("\n\ndistance: %lu\n", measured_distance);
#endif

		// determine line position
		int8_t measured_line_position = handle_line_position();
		// turn servo based on line position
		turn_servo(measured_line_position);

#ifdef DEBUG_MODE
		printf("\n\nline position: %d\n", measured_line_position);
#endif

		// control motor via wheel rpm
		float dc = pi_control();
		motor_pwm_set_duty(dc);

		osDelay(1);
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

