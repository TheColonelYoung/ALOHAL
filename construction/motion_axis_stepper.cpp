#include "motion_axis_stepper.hpp"

Motion_axis_stepper::Motion_axis_stepper(Stepper_motor *motor, double ratio, Motion_axis::Direction home):
    Motion_axis(ratio, home_direction), motor(motor)
{

}

long Motion_axis_stepper::Move(double shift){
    double steps = round(shift/ratio);
    if(valid_position){
        position += shift;
    }
    if (shift > 0){
        //motor->Move(Stepper_motor::Direction::Forward, steps);
    } else if (shift < 0){
        //motor->Move(Flip_direction(Direction()), steps*(-1));
    } else {
        return 0;
    }
    return steps;
}

long Motion_axis_stepper::GoTo(double target_position){
    // Cannot make GoTo from unknown position, Home first
    if(!valid_position){
        return -1;
    }
    double shift = position - target_position;
    return Move(shift);
}

void Motion_axis_stepper::Home(){
    motor->Run(home_direction,200);
    valid_position = true;
    position = 0;
}

void Motion_axis_stepper::Hard_stop(){
    motor->Hard_stop();
}

void Motion_axis_stepper::Soft_stop(){
    motor->Soft_stop();
}

void Motion_axis_stepper::Release(){
    motor->Release();
}
