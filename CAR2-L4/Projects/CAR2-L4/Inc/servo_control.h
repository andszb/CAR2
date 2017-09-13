/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H

#include "system_init.h"

void set_servo_angle(float angle);
float pd_control();

#endif /* __SERVO_CONTROL_H */
