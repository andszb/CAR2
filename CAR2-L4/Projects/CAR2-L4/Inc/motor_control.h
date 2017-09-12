/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#include "system_init.h"
#include "cmsis_os.h"

void motor_pwm_set_duty(float duty);

#endif /* __MOTOR_CONTROL_H */
