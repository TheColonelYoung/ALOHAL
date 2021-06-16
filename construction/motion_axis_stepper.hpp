/**
 * @file motion_axis_stepper.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 20.08.2019 B!
 */

#pragma once

#include <cmath>

#include "motor/stepper_motor.hpp"
#include "motion_axis.hpp"

/**
 * @brief   This class describes any linear or rotary axis, which can be powered by stepper motor.
 *          Axis works in dimensionless measuring units, but for moving mm and for rotation degrees are assumed
 *              But selection of unit does not effect class
 *
 *
 */
class Motion_axis_stepper: protected Motion_axis
{
private:
    Stepper_motor * motor = nullptr;

public:
    Motion_axis_stepper(Stepper_motor *motor, double ratio, Stepper_motor::Direction home = Stepper_motor::Direction::Reverse);

    /**
     * @brief Produce motion in given direction, relative position
     *
     * @param shift         Length or degree to make
     *                      Positive value if Forward, negative Reverse direction
     * @return long         Number of steps produced by motor
     */
    long Move(double shift);

    /**
     * @brief Produce motion to target position, absolute motion
     *
     * @param target_position   Distance from home position
     * @return long             Number of steps produced by motor
     */
    long GoTo(double target_position);


    long Run(double speed);

    /**
     * @brief Move motor to home position (until switch is closed)
     *
     * NOTE: Possible bug when is direction reversed
     * @return long
     */
    void Home();

    /**
     * @brief Perform hard stop at actual position
     */
    void Hard_stop();

    /**
     * @brief   Perform soft stop at actual position
     *          Decelerate motor from actual speed
     */
    void Soft_stop();

    /**
     * @brief   Disengage motor, In this state axis can freefly move
     *          This can lead to position loss,
     */
    void Release();


};

