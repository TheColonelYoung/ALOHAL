/**
 * @file TIM_channel.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.2
 * @date 01.04.2020
 */

#pragma once

#include "mcu/configuration/settings.hpp"
#include "global_includes.hpp"

#if   defined(MCU_FAMILY_STM32_F0)
# include "stm32f0xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F1)
# include "stm32f1xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F3)
# include "stm32f3xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F4)
# include "stm32f4xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F7)
# include "stm32f7xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L0)
# include "stm32l0xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L1)
# include "stm32l1xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L4)
# include "stm32l4xx_hal_tim.h"
#endif

#include <vector>
#include <string>

class Timer;

using namespace std;

class TIM_channel {
    int _index;
    uint32_t _address;

public:
    Timer *_parent_timer;

    TIM_channel(Timer *parent, uint32_t address);

    uint32_t  Address() const {return _address;};

    int Set_mode(string mode);

    int PWM_Generate(float percentage);
    void PWM_Start();
    void PWM_Stop();

};
