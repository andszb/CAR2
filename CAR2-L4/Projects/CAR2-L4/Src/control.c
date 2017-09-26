#include "control.h"
#include "servo_control.h"
#include "motor_control.h"
#include "proximity_driver.h"
#include "proximity_driver.h"

#define DEBUG_MODE

void control_thread()
{
	while (1) {

//		check for any object
		uint32_t measured_distance = read_proximity_data();
		process_proximity(measured_distance);
#ifdef DEBUG_MODE
		printf("\ndistance: %lu\n", measured_distance);
#endif
// 		determine line position
		get_line_sensor_data();

		turn_servo();
//		determine required rpm
		motor_pwm_set_duty(pi_control());
		osDelay(1000);
	}

	terminate_thread();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    	ovf_cntr++;
    else
    	cm_cntr++;
}

void terminate_thread()
{
	while (1)
		osThreadTerminate(NULL);
}

