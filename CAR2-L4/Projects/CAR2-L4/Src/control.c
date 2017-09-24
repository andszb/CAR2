#include "control.h"

#define DEBUG_MODE

uint8_t button_pressed = 0;

void control_thread()
{
	while (1) {
		if (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET) {
			if (!button_pressed) {
				required_rpm = 150;
				button_pressed = 1;
			} else {
				required_rpm = 0;
				button_pressed = 0;
			}
		}
//		check for any object
		uint32_t measured_distance = read_proximity_data();
		process_proximity(measured_distance);
#ifdef DEBUG_MODE
		printf("\ndistance: %lu\n", measured_distance);
#endif
// 		determine line position
		get_line_sensor_data();

		set_servo_angle(pd_control());
//		determine required rpm
		motor_pwm_set_duty(pi_control());
		osDelay(1000);
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

