/**
 * @file L6470.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 18.08.2019
 */

#pragma once

#include <string>
#include <vector>
#include <bitset>

#include "stepper_motor.hpp"
#include "spi/spi_device.hpp"

using namespace std;

class L6470: public Stepper_motor, public SPI_device{
private:

public:
    struct __attribute__((packed)) status{
        //Order is reversed against datasheet because of structure packaging
        // MS byte, bits 8 -> 15
        uint8_t WRONG_CMD   : 1; // Wrong command  (active - HIGH)
        uint8_t UVLO        : 1; // Under voltage lockout  (active - LOW)
        uint8_t TH_WRN      : 1; // Thermal warning event  (active - LOW)
        uint8_t TH_SD       : 1; // Thermal shutdown event (active - LOW)
        uint8_t OCD         : 1; // Overcurrent detection (active - LOW)
        uint8_t STEP_LOSS_A : 1; // Lost step on bridge A (active - LOW)
        uint8_t STEP_LOSS_B : 1; // Lost step on bridge B (active - LOW)
        uint8_t SCK_MOD     : 1; // Step_clock mode
        // L byte, bits 0 -> 7
        uint8_t HIZ         : 1; // Bridge high impedance state (active - HIGH)
        uint8_t BUSY        : 1; // Reflets BUSY pin status (idle - HIGH)
        uint8_t SW_F        : 1; // Switch input status  (LOW - open, HIGH - close)
        uint8_t SW_EVN      : 1; // Switch tur-on event (active - HIGH)
        Stepper_motor::Direction DIR    : 1; // Direction of motor
        Stepper_motor::Status MOT_STATUS: 2; // Motor status
        uint8_t NOTPERF_CMD : 1; // Command cannot be performed  (active - HIGH)
    };

    L6470() = default;

    using SPI_device::SPI_device;

    /**
     * @brief Stop stepper motor immediately
     */
    void Hard_stop() override;

    /**
     * @brief Stop motor with a deceleration phase
     *
     * @return int 0 if is it possible, -1 if not
     */
    int Soft_stop() override;

    /**
     * @brief       Motor will makes number of steps in defined direction
     *
     * @param dir   Direction for move
     * @param steps Number of steps to do
     * @param speed Speed of steps, if is set to 0 is used default speed of motor, parameter is used only for this command
     *              Does not revrite class variable, respects max and min speed
     * @return int  0 if is it possible, -1 if not (due to max or min speed)
     */
    int Move(Direction dir, uint steps, uint speed = 0) override;

    /**
     * @brief       Motor will make unlimited number of steps in defined direction
     *              Program must ensure stop conditions
     *
     * @param dir   Direction for move
     * @param speed Speed of steps, if is set to 0 is used default speed of motor, parameter is used only for this command
     *              Does not revrite class variable, respects max and min speed
     * @return int  0 is is it possible, -1 if not (due to max or min speed)
     */
    int Run(Direction dir, uint speed = 0) override;

    /**
     * @brief       Set motor into sleep state, High impedance state of MOSFETs
     *
     * @return int  0 if is it possible, -1 if not (disabled sleep state)
     */
    int Sleep() override;

    /**
     * @brief       Reset target stepper motor driver
     *
     * @return int  0 if is it possible, -1 if not (target did not supports reset)
     */
    int Reset() override;

    /**
     * @brief           Read status register from L6470
     *
     * @return uint16_t Constent of register, format of L6470::status
     */
    uint16_t Status();

    /**
     * @brief           Read status from L6470 and format it into string
     *
     * @return string   String repressentation of status register
     */
    string Status_formated();
};



