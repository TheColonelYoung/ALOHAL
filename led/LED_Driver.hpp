#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif

#include "timer/timers.hpp"
#include "gpio/pin.hpp"
#include "dac/DAC.hpp"

#include <vector>

using namespace std;

class LED_Driver {

private:
    bool enable = false;
    float current_limit_ma;

    int channel;
    float multiplicator;
    float value;
    Pin enable_pin;

public:


    LED_Driver(int Channel,Pin enable_pin_set, float current_limit_ma_set, float multiplicator_set);

    void Enable();
    void Disable();

    void Set_value(float value);

};

void LED_Driver_Init();
float LED_Driver_refine(float orig_value);

#endif // ifndef PWM_H
