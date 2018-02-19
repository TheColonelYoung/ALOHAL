#ifndef EXT_IT
#define EXT_IT

//------------------LIBRARY_OPTIONS----------------


//------------------LIBRARY_INCLUDES--------------

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
