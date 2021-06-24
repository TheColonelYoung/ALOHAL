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
#include "motor/stepper_motor.hpp"
#include "tools/tasker.hpp"
#include "rtos/utils.hpp"

/**
 * @brief Version of motion axis, which is powered by stepper motor driven by L6470 driver
 */
class Motion_axis_L6470: protected Motion_axis{
private:
    /**
     * @brief   Pointer to L6470 stepper motor driver which drives axis
     */
    L6470 * stepper_motor = nullptr;

    /**
     * @brief   Speed used for homing of axis
     *          Speed is in units of axis per second
     */
    double homing_speed;

public:
    Motion_axis_L6470(L6470 * stepper_motor, double ratio, double max_position, double homing_speed, Motion_axis::Direction home_direction = Motion_axis::Direction::Reverse, double min_position = 0);

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

    /**
     * @brief   Perform homing on axis which have endstop inside track of axis and not on its end
     */
    void Home_bidirectional();

    /**
     * @brief   Update position from stepper driver and use it as current position
     *          This method i used when axis is driven via speed commands
     *
     * @return double   Updated position
     */
    double Update_position();

    /**
     * @brief   Checks if is safe to run from actual position
     *          If valid position is not established, then all Run commands are allowed
     *
     * @param speed     Requested speed
     * @return true     Run command can be executed
     * @return false    Run command cannot be executed
     */
    bool Run_boundary_check(double speed);
};
