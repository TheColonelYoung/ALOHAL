/**
 * @file motion_axis.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.05.2021
 */

#pragma once

#include "modifiers/loggable.hpp"
#include "motor/stepper_motor.hpp"

/**
 * @brief   This class describes any linear or rotary axis, which can be powered by any type of motor.
 *          Axis works in dimensionless measuring units, but for moving mm and for rotation degrees are assumed
 *              But selection of unit does not effect class
 */
class Motion_axis : public Loggable
{
public:

    typedef Stepper_motor::Direction Direction;

protected:
    /**
     * @brief Actual position of motor, invalid until homing
     */
    double position = 0;

    /**
     * @brief   Determinates if position is valid
     */
    bool valid_position = false;

    /**
     * @brief  Movement or rotation, which will be produce when motor make 1 step
     */
    double ratio;

    /**
     * @brief   Direction of movement, where is installed endstop
     *          To this direction will be motor homing
     *          Direction to other side then homing is considered to be default direction (positive)
     */
    Motion_axis::Direction home_direction;

public:
    Motion_axis(double ratio, Motion_axis::Direction home_direction = Motion_axis::Direction::Reverse);

    /**
     * @brief   Produce motion in given direction, works with relative position
     *          example: position = 2, move = 3, final position = 5
     *
     * @param shift         Length or degree to make
     *                      Positive value if Forward, negative Reverse direction
     * @return long         Number of steps produced by motor
     */
    virtual bool Move(double shift) = 0;

    /**
     * @brief   Produce motion to target position, works with absolute position
     *          example: position = 2, move = 3, final position = 3
     *
     * @param target_position   Distance from home position
     * @return true             Position is reachable
     * @return false            Position is unreachable
     */
    virtual bool GoTo(double target_position) = 0;

    /**
     * @brief   Produce motion at given speed
     *          This motion is not restricted by any position limit of axis!
     *          Positive value is movement in default direction, negative in opposite direction
     *
     * @param speed     Speed in units per second
     */
    virtual void Run(double speed) = 0;

    /**
     * @brief   Move motor to home position
     *          This operation will set position to 0
     */
    virtual void Home() = 0;

    /**
     * @brief   Perform hard stop at actual position
     */
    virtual void Hard_stop() = 0;

    /**
     * @brief   Perform soft stop at actual position
     *          Decelerate motor from actual speed
     */
    virtual void Soft_stop() = 0;

    /**
     * @brief   Disengage motor, In this state axis can freefly move
     *          This can lead to position loss,
     */
    virtual void Release() = 0;

protected:

    /**
     * @brief Return default direction of movement
     *
     * @return Stepper_motor::Direction
     */
    Motion_axis::Direction Default_direction(){ return Flip_direction(home_direction); };

    /**
     * @brief Exchange Forward to Reverse and vice versa
     *
     * @param direction                 Original direction
     * @return Stepper_motor::Direction New direction
     */
    Motion_axis::Direction Flip_direction(Motion_axis::Direction direction);
};
