#include "stepper_motor.hpp"

StepperMotor::StepperMotor(float step_size, float speed)
    : step_size(step_size), speed(speed){
    step_time = 1000000 / (speed / step_size);
}

StepperMotor::StepperMotor(float step_size, float speed, float acceleration)
    : step_size(step_size), speed(speed), acceleration(acceleration){
    step_time = 1000000 / (speed / step_size);
    acc_en    = true;
    Acc_init_calc();
}

long StepperMotor::Move(double degrees, SM_Directions direction){
    if (direction == stop) {
        steps = 0;
        return 0;
    }
    long old_steps = steps;
    long step_inc  = degrees / step_size;
    if (direction == this->direction) {
        steps += step_inc;
    } else {
        if (step_inc > steps) {
            this->direction = direction;
            steps = step_inc - steps;
        } else if (step_inc < steps) {
            steps -= step_inc;
        }
    }
    if (steps == 0) {
        steps = 1;
    }

    if ((old_steps == 0) && acc_en) {
        Set_timer(acc_initial_step * 1000000);
        accelerating = true;
        acc_step     = acc_initial_step;
    } else if (accelerating) {
        ;
    } else {
        Set_timer(step_time);
    }

    timer.Enable_IRQ();
    return steps;
} // Move

void StepperMotor::Acc_step(){
    if (accelerating) {
        acc_step -= pow(acc_step, 3) * (acceleration + 1.f);
        long acc_step_time = acc_step * 1000000;
        if (acc_step_time < step_time) {
            accelerating = false;
            Set_timer(step_time);
            return;
        }
        Set_timer(acc_step_time);
    }
}

void StepperMotor::Set_timer(int step_length){
    timer.Time_set(step_length);
}

int StepperMotor::Speed(float speed){
    if (speed < 0) {
        return -1;
    } else if (speed == 0) {
        Stop();
        return -2;
    }
    this->speed = speed;
    step_time   = 1000000 / (speed / step_size);
    Set_timer(step_time);
    // NOTE change of speed in run works only for non-accelerating motors
    return static_cast<int>(speed);
}

int StepperMotor::Acceleration(float acceleration){
    if (acceleration < 0) {
        return -1;
    } else if (acceleration == 0) {
        acc_en       = false;
        acceleration = 0;
    }
    acc_en = true;
    this->acceleration = acceleration;
    Acc_init_calc();
    return 0;
}

void StepperMotor::Acc_init_calc(){
    acc_initial_step = 1.f / (sqrt(pow(1.f / (step_time + 1), 2.f) + 2.f * acceleration));
}

void StepperMotor::Stop(){
    if (acc_en) {
        accelerating = false;
    }
    steps = 0;
    timer.Disable_IRQ();
}

void StepperMotor::Init(){ }
