#ifndef ENDSTOP_HPP
#define ENDSTOP_HPP

// ------------------LIBRARY_OPTIONS----------------
// ------------------LIBRARY_OPTIONS----------------
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

#include "gpio/pin.hpp"

class Endstop {
    Pin input_pin;

public:
    Endstop() =default;
    Endstop(Pin input);

    int Set_pin(Pin new_pin);
    bool Read();
};

#endif
