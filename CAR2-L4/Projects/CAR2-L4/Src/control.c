#include "control.h"
#define DEBUG_MODE

void control_thread()
{
	while (1) {

		uint32_t measured_distance = read_proximity_data();
		process_proximity(measured_distance);
#ifdef DEBUG_MODE
	printf("distance: %lu\n", measured_distance);
#endif
// 		determine line position
		set_servo_angle(pd_control());
//		determine required rpm
		motor_pwm_set_duty(pi_control());
		osDelay(10);
	}

	terminate_thread();
}

