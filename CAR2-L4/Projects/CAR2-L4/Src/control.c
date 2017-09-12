#include "control.h"

float pdcontrol;
float picontrol;

void control_thread()
{
	while (1) {

		pdcontrol = pd_control();
		picontrol = pi_control();
// 		determine line position
		set_servo_angle(pdcontrol);
//		determine required rpm
		motor_pwm_set_duty(picontrol);
		osDelay(10);
	}

	terminate_thread();
}

