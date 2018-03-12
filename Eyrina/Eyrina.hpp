#ifndef EYRINA_HPP
#define EYRINA_HPP

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

#include <vector>

#include "globals.hpp"
#include "construction/linear_rail.hpp"

using namespace std;

enum Axis { X, Y, Z, F};

class Eyrina {
    vector<Linear_rail> rails;

    Endstop left_end;
    Endstop right_end;

public:
    Eyrina();

    int Move_axis(Axis axis, long distance);
    int Rotate_axis(Axis axis, float degrees );

};

#endif
