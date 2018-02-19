/*
 * Author:	TCY
 * Name:	Sleep STM32 library
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	14.2.2018
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * WakeUp Pin 1 on PA.00.
 * WakeUp Pin 2 on PC.13.
 * WakeUp Pin 3 on PE.06.(STM32F07x/STM32F09x)
 * WakeUp Pin 4 on PA.02.(STM32F07x/STM32F09x)
 * WakeUp Pin 5 on PC.05.(STM32F07x/STM32F09x)
 * WakeUp Pin 6 on PB.05.(STM32F07x/STM32F09x)
 * WakeUp Pin 7 on PB.15.(STM32F07x/STM32F09x)
 * WakeUp Pin 8 on PF.02.(STM32F07x/STM32F09x)
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 *
 * --------------------BUGS--------------------
 *
 * ++++++++++++++++++++TODO++++++++++++++++++++
 *
 **************************/
#ifndef SLEEP_HPP
#define SLEEP_HPP

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
#include <string>
#include <vector>

using namespace std;

namespace SLP {

    void Sleep();
    void Stop();
    void Standby();

}

#endif // ifndef SLEEP_HPP
