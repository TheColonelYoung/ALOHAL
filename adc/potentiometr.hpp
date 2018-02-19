#ifndef UART_H
#define UART_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif defined STM32_F1
# include "stm32f1xx_hal.h"
#elif defined STM32_F3
# include "stm32f3xx_hal.h"
#elif defined STM32_F4
# include "stm32f4xx_hal.h"
#elif defined STM32_F7
# include "stm32f7xx_hal.h"
#endif // ifdef STM32_F0

#include "adc/ADC.hpp"
#include <string>

using namespace std;

class potentiometr{

};


#endif // ifndef UART_H
