#ifndef LINEAR_RAIL_HPP
#define LINEAR_RAIL_HPP

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
#include "motor/stepper_motor.hpp"
#include "motor/SM_DRV8825.hpp"


using namespace std;

enum Direction { Right, Left };

class Linear_rail {

    StepperMotor *motor;

    bool invert = false; // // in default move to right is motor rotation to right, when true, move to left is caused by turning motor to right
    float ratio; // represend amount of shif on rail caused by turning motor by 1 degree

    Endstop left_end;
    Endstop right_end;
public:
    // Linear_rail() = default;
    Linear_rail( StepperMotor *motor, float ratio);
    Linear_rail( StepperMotor *motor, float ratio, Endstop left, Endstop right);

    vector<int> Endstop_read();
    int Endstop_read(Direction endstop_side);
    int Move(long distance);


    bool Invert(bool flag); // sets invertation flag

    void Endstop_hit_left();
    void Endstop_hit_right();
};

#endif // ifndef LINEAR_RAIL_HPP
