/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

#include "cmsis_os.h"
#include "servo_control.h"
#include "motor_control.h"
#include "proximity_driver.h"

extern uint32_t ovf_cntr;
extern uint32_t cm_cntr;

void control_thread();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void terminate_thread();

#endif /* __CONTROL_H */
