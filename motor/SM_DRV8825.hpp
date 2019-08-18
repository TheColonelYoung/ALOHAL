/*
 * Author:	TCY
 * Name:	Library for usage of SM driver DRV8825 for STM32
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	26.3.2018
 * Description:
 */


#ifndef SM_DRV8825_H
#define SM_DRV8825_H

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

#include <string>

#include "globals.hpp"
#include "motor/stepper_motor.hpp"

using namespace std;

// classes
class SM_DRV8825 : public STEP_DIR_SM {
    Pin _step_pin;
    Pin _direction_pin;
    Pin _reset_pin;
    Pin _sleep_pin;
    Pin _fault_pin;

public:
    SM_DRV8825() = default;
    SM_DRV8825(Pin step_pin, Pin dir_pin,float step_size, float speed);
    SM_DRV8825(Pin step_pin, Pin dir_pin,float step_size, float speed, float acceleration);

    int Set_pin(string pin_name, Pin pin_set);
    void Select_timer(Timer &timer) override;

    long Move(double degrees, SM_Directions direction) override;
    int Speed(float speed) override;
    void Step() override;

    void Reset();       // reset driver
    void Sleep();       // set driver to sleep
    void Wake_up();     // give up driver from sleep
    bool Fault_check(); // check for fault on FAULT pin
};

#endif // ifndef SM_DRV8825_H
