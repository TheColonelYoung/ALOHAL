#include "SM_DRV8825.hpp"

SM_DRV8825::SM_DRV8825(float step_size, float speed)
    : StepperMotor(step_size, speed * 2){
};

SM_DRV8825::SM_DRV8825(float step_size, float speed, float acceleration)
    : StepperMotor(step_size, speed * 2, acceleration){
};


int SM_DRV8825::Set_pin(string pin_name, Pin pin_set){
    if (pin_name == "step") {
        _step_pin = pin_set;
        return 1;
    } else if (pin_name == "direction") {
        _direction_pin == pin_set;
        return 2;
    } else if (pin_name == "reset") {
        _reset_pin == pin_set;
        _reset_pin.Set(1);
        return 3;
    } else if (pin_name == "sleep") {
        _sleep_pin == pin_set;
        return 4;
    } else if (pin_name == "fault") {
        _fault_pin == pin_set;
        return 5;
    }
    return -1;
}

void SM_DRV8825::Select_timer(Timer &timer){
    this->timer = timer;
    timer.IRQ.Register(this, &SM_DRV8825::Step);
}

void SM_DRV8825::Step(){
    if (steps != 0) {
        _step_pin.Toggle();
        // steps are limited
        if (steps > 0) {
            steps -= 1;
            // steps decresed to 0 => set driver to sleep
            if (steps <= 0) {
                //Sleep();
                Stop();
            }
        }
    }
    if (acc_en) {
        Acc_step();
    }
}

long SM_DRV8825::Move(double degrees, SM_Directions direction){
    StepperMotor::Move(degrees *2, direction);
    return steps;
}

int SM_DRV8825::Speed(float speed){
    return StepperMotor::Speed(speed * 2);
}

void SM_DRV8825::Sleep(){
    _sleep_pin.Set(0);
}

void SM_DRV8825::Wake_up(){
    _sleep_pin.Set(1);
}

void SM_DRV8825::Reset(){
    _reset_pin.Set(0);
}

bool SM_DRV8825::Fault_check(){
    return _fault_pin.Read();
}
