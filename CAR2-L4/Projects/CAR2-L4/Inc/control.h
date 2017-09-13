/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

#include "cmsis_os.h"
#include "servo_control.h"
#include "motor_control.h"
#include "proximity_driver.h"
#include "main.h"

void control_thread();

#endif /* __CONTROL_H */