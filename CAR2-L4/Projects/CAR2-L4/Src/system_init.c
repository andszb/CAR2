/*
 * system_init.c
 *
 *  Created on: 2017. szept. 11.
 *      Author: AND
 */
#include "system_init.h"


//call init functions
int8_t system_init()
{
	pin_init();


	return 0;
}

//call port init functions
int8_t pin_init()
{
	portA_init();
	return 0;
}

// init portA pins
int8_t portA_init()
{

	return 0;
}
