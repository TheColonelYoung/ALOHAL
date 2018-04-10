#include "linear_rail.hpp"

Linear_rail::Linear_rail(StepperMotor *motor, float ratio) : motor(motor), ratio(ratio)
{ }

Linear_rail::Linear_rail(StepperMotor *motor, float ratio, Endstop left_end, Endstop right_end) : motor(motor), ratio(ratio), left_end(left_end), right_end(right_end)
{ }

int Linear_rail::Move(long distance){
    if (distance == 0) {
        return -1;
    }
    SM_Directions dir;
    if (distance > 0) {
        dir = right;
    } else {
        dir = left;
    }
    motor->Move(distance / ratio, dir);

    return 0;
}

bool Linear_rail::Invert(bool flag){
    invert = flag;
    return invert;
}
