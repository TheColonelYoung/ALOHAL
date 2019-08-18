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
 *********++++++++++++++++++++++TODO++++++++++++++++++++
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

#include <cmath>

#include "gpio/pin.hpp"
#include "timer/timer.hpp"

using namespace std;

enum SM_Directions { left = -1, stop = 0, right = 1 };

class STEP_DIR_SM {
protected:
    SM_Directions direction = stop;
    long steps = 0;    // number of remaning steps, -1 is infinite
    float step_size;   // size of step in degrees
    float speed   = 0; // actual speed in degrees in second
    int step_time = 0; // time between two steps in us

    bool acc_en       = false;
    bool accelerating = false;
    float acceleration; // in degrees per second square
    double acc_initial_step;
    double acc_step;

    Timer timer;

protected:
    void Set_timer(int step_length);

public:
    STEP_DIR_SM() = default;
    STEP_DIR_SM(float step_size, float speed);
    STEP_DIR_SM(float step_size, float speed, float acceleration);

    int Acceleration(float acceleration);
    void Acc_init_calc();
    int Acc_increment_calc();
    void Acc_step();

    void Init();    // Enable IRQ, initiate calculations
    void Stop();    // Stop steper motor rotation
    void Disable(); // Disable IRQ, need Init for another run

    virtual void Select_timer(Timer &timer) = 0; // Assign timer for motor
    virtual void Step() = 0;                     // Make one step by direction

    virtual long Move(double degrees, SM_Directions direction);
    virtual int Speed(float speed);
};

#endif // ifndef SM_H
