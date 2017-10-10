/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "cmsis_os.h"

void motor_pwm_set_duty(float duty);
float pi_control();
void go();
void stop();

#endif /* __MOTOR_CONTROL_H */
