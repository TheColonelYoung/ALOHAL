/**
 * @file motion_axis.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 20.08.2019 B!
 */

#pragma once

#include <cmath>

#include "motor/stepper_motor.hpp"

/**
 * @brief   This class describes any linear or rotary axis, which can be
 *          powered by stepper motor.
 *
 */
class Motion_axis
{
private:
    Stepper_motor *motor;

    /**
     * @brief Actual position of motor, invalid until homing
     */
    double position = -1;

    bool valid_position = false;

    /**
     * @brief   Direction of movement, where is installed endstop
     *          To this direction will be motor homing
     */
    Stepper_motor::Direction home_direction;

    /**
     * @brief  Movement or rotation, which will be produce when motor make 1 step
     */
    double ratio;

    bool reversed_direction = false;

public:
    Motion_axis() = default;
    Motion_axis(Stepper_motor *motor, double ration, Stepper_motor::Direction home = Stepper_motor::Direction::Reverse);

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

    /**
     * @brief Move motor to home position (until switch is closed)
     *
     * NOTE: Possible bug when is direction reversed
     * @return long
     */
    void GoHome();

    /**
     * @brief Reverse motor direction
     *
     * @param reverse
     */
    void Reverse(bool reverse){reversed_direction = reverse;}

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
     * @brief Perform sleep on axis
     */
    void Sleep();

private:
    /**
     * @brief Return default direction of movement
     *
     * @return Stepper_motor::Direction
     */
    Stepper_motor::Direction Direction();

    /**
     * @brief Exchange Forward to Reverse and vice versa
     *
     * @param direction                 Original direction
     * @return Stepper_motor::Direction New direction
     */
    Stepper_motor::Direction Flip_direction(Stepper_motor::Direction direction);


};

