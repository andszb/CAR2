/*
 * proximity_driver.h
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */

#ifndef __PROXIMITY_DRIVER_H_
#define __PROXIMITY_DRIVER_H_

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"


uint8_t object_flag;
uint32_t distance;

uint32_t read_proximity_data();
uint8_t process_proximity(uint32_t distance);
int8_t proximity_control_thread();

#endif /* APPLICATIONS_USER_PROXIMITY_DRIVER_H_ */
