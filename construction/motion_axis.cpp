#include "motion_axis.hpp"

Motion_axis::Motion_axis(Stepper_motor *motor, double ratio, Stepper_motor::Direction home)
    :motor(motor),home_direction(home),ratio(ratio)
{

}

long Motion_axis::Move(double shift){
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


long Motion_axis::GoTo(double target_position){
    // Cannot make GoTo from unknown position, Home first
    if(!valid_position){
        return -1;
    }
    double shift = position - target_position;
    return Move(shift);
}

void Motion_axis::GoHome(){
    motor->Run(home_direction,1000);
    valid_position = true;
    position = 0;
}

void Motion_axis::Hard_stop(){
    motor->Hard_stop();
}

void Motion_axis::Soft_stop(){
    motor->Soft_stop();
}

void Motion_axis::Sleep(){
    motor->Sleep();
}

Stepper_motor::Direction Motion_axis::Direction(){
    if (!reversed_direction){
        return static_cast<Stepper_motor::Direction>(0);
    } else {
        return static_cast<Stepper_motor::Direction>(1);
    }
}

Stepper_motor::Direction Motion_axis::Flip_direction(Stepper_motor::Direction direction){
    if (direction == Stepper_motor::Direction::Forward){
        return Stepper_motor::Direction::Reverse;
    } else {
        return Stepper_motor::Direction::Forward;
    }
}
