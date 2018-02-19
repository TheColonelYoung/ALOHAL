#ifndef STEPPER_H
#define STEPPER_H


#include "gpio/pin.hpp"
#include "uart/uart.hpp"
#include <math>

//#include "stm32f0xx_hal.h"

//--------------------LIBRARY OPTIONS-------------------
#define BASE_STEP_ANGLE 1.8
#define MOTOR_NUMBER 14
#define FREQ_CLK1 1000000
#define THREAD_PITCH 1.25 //stoupání závitu, posun závitu při otočení o 360°



int SM_out;
int SM_steps;

int SM_step();
int init_motor();

//---------------MICROSTEPPING-------------
//	Code	Step	Number	Accuracy(um)?
//	000		1		0		10
//	001		1/2		1		5
//	010		1/4		2		2.5
//	011		1/8		3		1.25
//	100		1/16	4		0.625
//	101		1/32	5		0.3125

#endif
