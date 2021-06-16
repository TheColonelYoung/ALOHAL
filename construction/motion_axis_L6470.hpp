/**
 * @file motion_axis_L6470.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.05.2021
 */

#pragma once

#include "globals.hpp"
#include "motion_axis.hpp"
#include "motor/L6470.hpp"
#include "tools/tasker.hpp"

class Motion_axis_L6470: protected Motion_axis{
private:
    L6470 * stepper_motor = nullptr;

public:
    Motion_axis_L6470(L6470 * stepper_motor, double ratio, Motion_axis::Direction home_direction = Motion_axis::Direction::Reverse);

    virtual bool Move(double shift) override final;

    virtual bool GoTo(double target_position) override final;

    virtual void Run(double speed) override final;

    virtual void Home() override final;

    virtual void Hard_stop() override final;

    virtual void Soft_stop() override final;

    virtual void Release() override final;

    /**
     * @brief   Runs stepper motor at low speed until end stop is released
     *          Direction of movement is opposite to home direction
     */
    void Release_switch();

};
