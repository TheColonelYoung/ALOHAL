#include "motion_axis_L6470.hpp"

Motion_axis_L6470::Motion_axis_L6470(L6470 *stepper_motor, double ratio, Motion_axis::Direction home_direction):
    Motion_axis(ratio, home_direction), stepper_motor(stepper_motor){ }


bool Motion_axis_L6470::Move(double shift){
    double steps = round(shift/ratio*stepper_motor->Microsteps());
    if(valid_position){
        position += shift;
    }
    if (shift > 0){
        stepper_motor->Move(Default_direction(), steps);
    } else if (shift < 0){
        stepper_motor->Move(Flip_direction(Default_direction()), steps*(-1));
    } else {
        return 0;
    }
    return steps;
}

bool Motion_axis_L6470::GoTo(double target_position){
    // Cannot make GoTo from unknown position, Home first
    if(!valid_position){
        return -1;
    }
    double shift = target_position - position;
    return Move(shift);
}

void Motion_axis_L6470::Run(double speed){
    if (speed > 0){
        stepper_motor->Run(Default_direction(), static_cast<unsigned int>(speed/ratio));
    } else if (speed < 0){
        stepper_motor->Run(Flip_direction(Default_direction()), static_cast<unsigned int>((speed*-1)/ratio));
    } else if (speed == 0){
        stepper_motor->Release();
    }
}

void Motion_axis_L6470::Home(){
    stepper_motor->Run(home_direction, 200);

    tasker()->Poll_until<true>(
        new Invocation_wrapper<L6470, bool, void>(stepper_motor, &L6470::Switch_status),
        1000 * 1000,
        new Invocation_wrapper<void, void, void>(
            new function<void()>(
                [this]() -> void{
                    Log_line(Log_levels::Debug, "Release switch")
                    this->Release_switch();
                }
            )
        )
    );
    position = 0;
    valid_position = true;
}

void Motion_axis_L6470::Release_switch(){
    stepper_motor->ReleaseSW(Flip_direction(home_direction));
}

void Motion_axis_L6470::Hard_stop(){
    stepper_motor->Hard_stop();
}

void Motion_axis_L6470::Soft_stop(){
    stepper_motor->Soft_stop();
}

void Motion_axis_L6470::Release(){
    stepper_motor->Release();
}
