#include "control.h"


void control_thread()
{
	while (1) {
// 		determine line position
		set_servo_angle(pd_control());
//		determine required rpm
		motor_pwm_set_duty(pi_control());
		osDelay(10);
	}

	terminate_thread();
}

