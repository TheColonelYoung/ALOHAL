#ifndef LINEAR_RAIL_HPP
#define LINEAR_RAIL_HPP

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
#include "gpio/endstop.hpp"
#include "motor/StepperMotor.hpp"


using namespace std;

enum Direction {Right, Left};

class Linear_rail {

    Endstop left_end;
    Endstop right_end;
    StepperMotor motor;

public:
    Linear_rail()=default;

    vector<int> Endstop_read();
    int Endstop_read(Direction endstop_side);

    int Move(long distance);

    void Endstop_hit_left();
    void Endstop_hit_right();
};

#endif
