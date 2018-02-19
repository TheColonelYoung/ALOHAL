/*
 * Author:	TCY
 * Name:	Base stepper motor library for STM32
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	27.7.2017
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 *
 * --------------------BUGS--------------------
 *
 *++++++++++++++++++++++TODO++++++++++++++++++++
 *
 **********************************************/
#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

// c++ libraries

// libraries needed for this library to run
#ifdef STM32_F0
# include "stm32f0xx_hal.h"
# include "stm32f0xx_hal_uart.h"
#elif defined STM32_F1
# include "stm32f1xx_hal.h"
# include "stm32f1xx_hal_uart.h"
#elif defined STM32_F3
# include "stm32f3xx_hal.h"
# include "stm32f3xx_hal_uart.h"
#elif defined STM32_F4
# include "stm32f4xx_hal.h"
# include "stm32f4xx_hal_uart.h"
#elif defined STM32_F7
# include "stm32f7xx_hal.h"
# include "stm32f7xx_hal_uart.h"
#endif // ifdef STM32_F0

#include "gpio/pin.h"
//#include "tasker.h"

// libraries nedded for running of efects this library and DEBUG

// defines

// externs
extern int SM_ID;

// globals
// Id counter for generatin new ID for new motor

// declaration

using namespace std;

// function prototypes
int SM_Get_freeID(); // give unique id to new stepper motor

// classes
class StepperMotor {
private:


protected:
	int _id;             // unique ID for every motor

public:
	bool direction = 0; //
	int speed;          // actual speed in degrees in second
	int steps;          // number of remaning steps

public:
	void Step();              // make one step by direction
	void Direction(bool dir); // set direction 1-left, 0-right
	void Set_speed(int dps);  // new speed in degree per second
	int Get_ID();             // return ID of motor
	void Start();             // start stepper motor rotation
	void Stop();              // stop steper motor rotation
};

#endif // ifndef SM_H
