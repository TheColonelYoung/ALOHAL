/**
 * @file stepper_motor.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 18.08.2019
 */

#pragma once

#include "vector"

typedef unsigned int uint;
using namespace std;

/**
 * @brief Generic pure virtual class for stepper motor
 *
 */
class Stepper_motor
{
public:
    enum Status{
        Stopped         = 0,
        Acceleration    = 1,
        Deceleration    = 2,
        Constant_speed  = 3
    };

    enum Direction: bool{
        Reverse = 0,
        Forward = 1,
    };

private:
    uint speed;

    /**
     * @brief   Enable acceleration and deceleration phases
     *          Speed of acceleration and deceleration is below
     *          If set to 0 is acceleration and deceleration also disabled (this allows disabling only one ramp)
     */
    bool ramp_speed_enable = true;
    uint acceleration;
    uint deceleration;

    /**
     * @brief   Restrictions for speed, are non mandatory
     *          If is ramp speed is enabled, accekeration start at min_speed and stop at max_speed
     *          Deceleration starts at current speed and stops at min_speed, as next motor is hard stopped
     */
    uint max_speed;
    uint min_speed;

    /**
     * @brief Actual status and direction of stepper motor
     */
    Status status;
    Direction direction;

    /**
     * @brief If target device support sleep state (high impedance state of MOSFET drivers)
     */
    bool sleep_enable = true;

    /**
     * @brief If target device support reset or if can be reset done
     */
    bool reset_enable = false;

public:

    Stepper_motor() = default;

    /**
     * @brief Stop stepper motor immediately
     */
    virtual void Hard_stop() = 0;

    /**
     * @brief Stop motor with a deceleration phase
     *
     * @return int 0 if is it possible, -1 if not
     */
    virtual int Soft_stop() = 0;

    /**
     * @brief       Motor will makes number of steps in defined direction
     *
     * @param dir   Direction for move
     * @param steps Number of steps to do
     * @param speed Speed of steps, if is set to 0 is used default speed of motor, parameter is used only for this command
     *              Does not revrite class variable, respects max and min speed
     * @return int  0 if is it possible, -1 if not (due to max or min speed)
     */
    virtual int Move(Direction dir, uint steps, uint speed = 0) = 0;

    /**
     * @brief       Motor will make unlimited number of steps in defined direction
     *              Program must ensure stop conditions
     *
     * @param dir   Direction for move
     * @param speed Speed of steps, if is set to 0 is used default speed of motor, parameter is used only for this command
     *              Does not revrite class variable, respects max and min speed
     * @return int  0 is is it possible, -1 if not (due to max or min speed)
     */
    virtual int Run(Direction dir, uint speed = 0) = 0;

    /**
     * @brief       Disable motor MOSFETs, low power consumption and heating
     *              In this state motor can freefly move, is disengage
     *              This can lead to position loss
     *              This mode is sometimes called Sleep
     *
     * @return int  0 if is it possible, -1 if not (disabled sleep state)
     */
    virtual int Release() = 0;

    /**
     * @brief       Reset target stepper motor driver
     *
     * @return int  0 if is it possible, -1 if not (target did not supports reset)
     */
    virtual int Reset() = 0;

};

