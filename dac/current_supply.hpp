#ifndef CURRENT_SUPPLY_H
#define CURRENT_SUPPLY_H

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

#include <vector>

#include "globals.hpp"
#include "dac/DAC.hpp"

using namespace std;

class Current_supply {
    DA_C_channel *_DAC_chan; // Output channel of DAC

    uint _sense_resistor; // Value of sense resistor in mR
    uint _limit;          // Limit of current in uA

    uint _max_current; // Maximal current which can be achived

public:
    Current_supply() = default;
    Current_supply(DA_C_channel *DAC_chan, uint sense);
    Current_supply(DA_C_channel *DAC_chan, uint sense, uint limit);

    int Set_current(uint current_ua); // Set value of current in uA
};


#endif // ifndef DAC_H
