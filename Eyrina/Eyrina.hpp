#ifndef EYRINA_HPP
#define EYRINA_HPP

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
#include "motor/SM_DRV8825.hpp"

using namespace std;

#define EYRINA_SM_SPEED        5000
#define EYRINA_SM_ACCELERATION 4000
#define EYRINA_SM_STEP_SIZE    1.8

#define EYRINA_LR_COUNT        4
#define EYRINA_LR_SHIFT_RATIO  1.389

class Eyrina {
    vector<Linear_rail> rails;

public:
    enum Axis { X = 0, Y, Z, F };

    Eyrina();
    void Init_rails();

    int Move_axis(Axis axis, long distance);
    int Rotate_axis(Axis axis, float degrees);
};

#endif // ifndef EYRINA_HPP
