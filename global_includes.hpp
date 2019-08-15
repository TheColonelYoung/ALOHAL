#ifndef GLOBAL_INCLUDES_HPP
#define GLOBAL_INCLUDES_HPP

#include "mcu/settings.hpp"

#if   defined(MCU_FAMILY_STM32_F0)
# include "stm32f0xx_hal.h"
#elif defined(MCU_FAMILY_STM32_F1)
# include "stm32f1xx_hal.h"
#elif defined(MCU_FAMILY_STM32_F3)
# include "stm32f3xx_hal.h"
#elif defined(MCU_FAMILY_STM32_F4)
# include "stm32f4xx_hal.h"
#elif defined(MCU_FAMILY_STM32_F7)
# include "stm32f7xx_hal.h"
#elif defined(MCU_FAMILY_STM32_L0)
# include "stm32l0xx_hal.h"
#elif defined(MCU_FAMILY_STM32_L1)
# include "stm32l1xx_hal.h"
#elif defined(MCU_FAMILY_STM32_L4)
# include "stm32l4xx_hal.h"
#endif

#endif
