#ifndef TIM_CHANNEL_HPP
#define TIM_CHANNEL_HPP

#include "mcu/configuration/settings.hpp"

#if   defined(MCU_FAMILY_STM32_F0)
# include "stm32f0xx_hal.h"
# include "stm32f0xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F1)
# include "stm32f1xx_hal.h"
# include "stm32f1xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F3)
# include "stm32f3xx_hal.h"
# include "stm32f3xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F4)
# include "stm32f4xx_hal.h"
# include "stm32f4xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F7)
# include "stm32f7xx_hal.h"
# include "stm32f7xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L0)
# include "stm32l0xx_hal.h"
# include "stm32l0xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L1)
# include "stm32l1xx_hal.h"
# include "stm32l1xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L4)
# include "stm32l4xx_hal.h"
# include "stm32l4xx_hal_tim.h"
#endif

#include <vector>
#include <string>

class Timer;

using namespace std;

class TIM_channel {

    int _index;


public:
    Timer *_parent_timer;
    uint32_t _address;

    TIM_channel(Timer *parent, uint32_t address);

    int Set_mode(string mode);

    int PWM_Generate(float percentage);
    void PWM_Start();
    void PWM_Stop();
};

#include "timer/timer.hpp"


#endif // ifndef TIM_CHANNEL_HPP
