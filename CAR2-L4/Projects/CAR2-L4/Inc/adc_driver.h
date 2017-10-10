/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include "stdio.h"
#include <stdlib.h>
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"

//define sensor data structure
typedef struct {
	int16_t sensor1_data;
	int16_t sensor2_data;
	int16_t sensor3_data;
	int16_t sensor4_data;
	int16_t sensor5_data;
	int16_t sensor6_data;
	int16_t sensor7_data;
	int16_t sensor8_data;
	int16_t sensor9_data;
} sensor_data_t;

sensor_data_t sensor_data;
int8_t no_line_flag;

int8_t handle_line_position();

#endif /* __ADC_DRIVER_H */
