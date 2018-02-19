/*
 * Author:	TCY
 * Name:	Library for usage of SM driver DRV8825 for STM32
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	1.7.2017
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 *
 * --------------------BUGS--------------------
 *++++++++++++++++++++++ TODO++++++++++++++++++++
 *
 *
 **********************************************/
#ifndef SM_DRV8825_H
#define SM_DRV8825_H

// c++ libraries
#include <string>

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


#include "motor/StepperMotor.hpp"
#include "timer/tasker.hpp"
#include "misc/text_transform.hpp"

// libraries nedded for function of tasker events and DEBUG

// defines

// declaration
class Tasker;
class SM_DRV8825;

// externs
extern vector<SM_DRV8825> SM_DRV8825_stash;
extern Tasker tasker;

using namespace std;

// function prototypes
void SM_DRV8825_Finder();

// classes
class SM_DRV8825 : public StepperMotor {
	Pin _direction_pin;
	Pin _step_pin;
	Pin _reset_pin;
	Pin _sleep_pin;
	Pin _fault_pin;

public:
	SM_DRV8825(int speed_set, bool direction_set, int steps_set, Pin step_pin_set, Pin direction_pin_set, Pin reset_pin_set, Pin sleep_pin_set, Pin fault_pin_set);


	void Step();
	void Reset();                 // reset driver
	void Sleep();                 // set driver to sleep
	void Wake_up();               // give up driver from sleep
	bool Fault_check();           // check for fault on FAULT pin
	void Set_direction(bool dir); // set direction, change pin and var value
};

#endif // ifndef SM_DRV8825_H
