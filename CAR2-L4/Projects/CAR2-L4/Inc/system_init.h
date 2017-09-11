 /*
 *  Created on: 2017. szept. 11.
 *      Author: AND
 */

#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

int8_t pin_init();
int8_t portA_init();
int8_t portB_init();
int8_t portD_init();
int8_t systeminit();

#endif
