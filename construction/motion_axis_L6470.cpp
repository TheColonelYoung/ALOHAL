#include "motion_axis_L6470.hpp"

Motion_axis_L6470::Motion_axis_L6470(L6470 *stepper_motor, double ratio, double max_position, double homing_speed, Motion_axis::Direction home_direction, double min_position) :
    Motion_axis(ratio, max_position, min_position, home_direction), stepper_motor(stepper_motor), homing_speed(homing_speed){
    if (min_position < 0){
        stepper_motor->Switch_disable();
    }
}

bool Motion_axis_L6470::Move(double shift){
    double steps = round(shift / ratio * stepper_motor->Microsteps());
    if (valid_position) {
        if ((position + shift > max_position) or (position + shift < min_position)) {
            Log_line(Log_levels::Error, "Target position is out of range");
            return 0;
        }
        position += shift;
    } else {
        Log_line(Log_levels::Warning, "Moving without known position");
    }
    if (shift > 0) {
        stepper_motor->Move(Default_direction(), steps);
    } else if (shift < 0) {
        stepper_motor->Move(Flip_direction(Default_direction()), steps * (-1));
    } else {
        return 0;
    }
    return steps;
}

bool Motion_axis_L6470::GoTo(double target_position){
    // Cannot make GoTo from unknown position, Home first
    if (!valid_position) {
        Log_line(Log_levels::Error, "Cannot move to absolute position without homing");
        return -1;
    }
    double shift = target_position - position;
    return Move(shift);
}

void Motion_axis_L6470::Run(double speed){
    if (!Run_boundary_check(speed)){
        stepper_motor->Soft_stop();
        return;
    }
    if (speed > 0) {
        stepper_motor->Run(Default_direction(), static_cast<unsigned int>(speed / ratio));
    } else if (speed < 0) {
        stepper_motor->Run(Flip_direction(Default_direction()), static_cast<unsigned int>((speed * -1) / ratio));
    } else if (speed == 0) {
        stepper_motor->Soft_stop();
    }
}

bool Motion_axis_L6470::Run_boundary_check(double speed){
    // Reserve in fraction of axis length (from max and min position) in which new Run command cannot be executed
    const double save_track_reserve = 0.05;
    // Reserve in fraction of axis length (from home position) in which new Run command cannot be executed
    // Only used when axis track has no negative part of track (min_position == 0)
    const double home_track_reserve = 0.05;
    // Update actual position from stepper driver before calculations
    Update_position();
    if (valid_position){
        // Near max position
        if ((position > ((1-save_track_reserve) * max_position)) and (speed > 0)){
            return false;
        }
        // Near min position
        if ((position < ((1-save_track_reserve) * min_position)) and (speed < 0)){
            return false;
        }
        // No negative track length, near to home position
        if ((min_position == 0) and (position < (home_track_reserve * max_position)) and (speed < 0)){
            return false;
        }
    }
    return true;
}

double Motion_axis_L6470::Update_position(){
    double new_position = stepper_motor->Position()*ratio/stepper_motor->Microsteps();
    if (home_direction == Motion_axis::Direction::Forward){
        new_position *= -1;
    }
    position = new_position;
    valid_position = true;
    return position;
}

void Motion_axis_L6470::Home(){
    if (min_position < 0) {
        Home_bidirectional();
        return;
    }
    stepper_motor->Status();
    // If switch is not active, run until is activated
    if (!stepper_motor->Switch_status()) {
        stepper_motor->Switch_enable();
        stepper_motor->Run(home_direction, homing_speed / ratio);
        // Wait for endstop switch hit
        RTOS::Poll_until<true>(Invocation_wrapper<L6470, bool, void>(stepper_motor, &L6470::Switch_event), 500);
    }
    // Release endstop switch
    Log_line(Log_levels::Debug, "Release switch")
    this->Release_switch();
    // Wait until is switch released
    RTOS::Poll_until<false>(Invocation_wrapper<L6470, bool, void>(stepper_motor, &L6470::Switch_status), 200);
    // Set home position and enable absolute movement
    stepper_motor->SetHome();
    position       = 0;
    valid_position = true;
}

void Motion_axis_L6470::Home_bidirectional(){
    double length = (min_position - max_position) * -1;
    const unsigned int sections = 8;
    double section_length       = length / sections;
    //Backup maximal speed and set speed for homing
    unsigned int max_speed_of_motor = stepper_motor->Stepper_motor::Max_speed();
    stepper_motor->Max_speed(homing_speed / ratio);
    // Clear status register, enable switch (motor must be disabled when updating config - HiZ)
    valid_position = false;
    stepper_motor->Status();
    stepper_motor->Soft_HiZ();
    stepper_motor->Switch_enable();
    stepper_motor->Soft_stop();
    // Check if switch is not already pressed
    if (!stepper_motor->Switch_status()) {
        for (size_t i = 1; i <= sections; i++) {
            if (i % 2) {
                Move(-1 * section_length * i);
            } else  {
                Move(section_length * i);
            }
            RTOS::Poll_until<false>(Invocation_wrapper<L6470, bool, void>(stepper_motor, &L6470::Busy), 100);

            if (stepper_motor->Switch_status()) {
                stepper_motor->Hard_stop();
                break;
            }
            // direction = Flip_direction(direction);
            if (i == sections) {
                Log_line(Log_levels::Fatal, "Invalid homing");
            }
        }
    }
    // Release endstop switch
    Log_line(Log_levels::Debug, "Release switch")
    this->Release_switch();
    // Wait until is switch released
    RTOS::Poll_until<false>(Invocation_wrapper<L6470, bool, void>(stepper_motor, &L6470::Switch_status), 100);
    // Set home position and enable absolute movement
    stepper_motor->SetHome();
    position       = 0;
    valid_position = true;
    // Restore maximal speed of motor, disable switch (motor must be disabled when updating config - HiZ)
    stepper_motor->Soft_HiZ();
    stepper_motor->Switch_disable();
    stepper_motor->Soft_stop();
    stepper_motor->Max_speed(max_speed_of_motor);
} // Motion_axis_L6470::Home_bidirectional

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
    valid_position = false;
    stepper_motor->Release();
}
