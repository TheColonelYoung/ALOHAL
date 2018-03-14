#ifndef FLASH_HPP
#define FLASH_HPP

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
# include "stm32f0xx_hal_flash.h"
# include "stm32f0xx_hal_flash_ex.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
# include "stm32f1xx_hal_flash.h"
# include "stm32f1xx_hal_flash_ex.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
# include "stm32f3xx_hal_flash.h"
# include "stm32f3xx_hal_flash_ex.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
# include "stm32f4xx_hal_flash.h"
# include "stm32f4xx_hal_flash_ex.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
# include "stm32f7xx_hal_flash.h"
# include "stm32f7xx_hal_flash_ex.h"
#endif // ifdef STM32_F0

using namespace std;

#include <string>
#include <vector>

#include "globals.hpp"

#define FLASH_SIZE_ADDRESS FLASHSIZE_BASE

#define ADC_CHANNEL_CONCAT(channel) (ADC_CHANNEL_ ## channel)

class Flash_mem {
    uint _size;
    uint _pages;

public:
    Flash_mem();

    int Size();      // size of whole flash of STM32 in kB
    int Pages();     // number of pages in memory
    int Page_size(); // size of one page in kB

    int Page_erase(int page);
};

#endif // ifndef FLASH_HPP
