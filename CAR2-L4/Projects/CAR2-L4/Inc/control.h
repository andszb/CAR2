/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

#include "cmsis_os.h"
#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"

extern uint32_t ovf_cntr;
extern uint32_t cm_cntr;

void control_thread();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void terminate_thread();

#endif /* __CONTROL_H */
