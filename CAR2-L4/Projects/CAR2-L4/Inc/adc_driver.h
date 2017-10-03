/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include "stdio.h"
#include <stdlib.h>
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"

void handle_line_position();

int8_t line_position;
int8_t no_line_flag;

#endif /* __ADC_DRIVER_H */
