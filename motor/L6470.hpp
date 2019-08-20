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
#include <cmath>
#include <map>

#include "stepper_motor.hpp"
#include "spi/spi_device.hpp"

using namespace std;

/**
 * @brief Datasheet https://www.st.com/resource/en/datasheet/l6470.pdf
 */
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

    enum class register_map: uint8_t{
        ABS_POS     = 0x01,
        EL_POS      = 0x02,
        MARK        = 0x03,
        SPEED       = 0x04,
        ACC         = 0x05,
        DEC         = 0x06,
        MAX_SPD     = 0x07,
        MIN_SPD     = 0x08,
        FS_SPD      = 0x15,
        KVAL_HOLD   = 0x09,
        KVAL_RUN    = 0x0a,
        KVAL_ACC    = 0x0b,
        KVAL_DEC    = 0x0c,
        INT_SPD     = 0x0d,
        ST_SLP      = 0x0e,
        FN_SLP_ACC  = 0x0f,
        FN_SLP_DEC  = 0x10,
        K_THERM     = 0x11,
        ADC_OUT     = 0x12,
        OCD_TH      = 0x13,
        STALL_TH    = 0x14,
        STEP_MODE   = 0x16,
        ALARM_EN    = 0x17,
        CONFIG      = 0x18,
        STATUS      = 0x19
    };

    enum class command: uint8_t{
        NOP         = 0b00000000,
        SetParam    = 0b00000000,
        GetParam    = 0b00100000,
        Run         = 0b01010000,
        StepClock   = 0b01011000,
        Move        = 0b01000000,
        GoTo        = 0b01100000,
        GoToDIR     = 0b01101000,
        GoUntil     = 0b10000010,
        ReleaseSW   = 0b10010010,
        GoHome      = 0b01110000,
        GoMark      = 0b01111000,
        ResetPos    = 0b11011000,
        ResetDevice = 0b11000000,
        SoftStop    = 0b10110000,
        HardStop    = 0b10111000,
        SoftHiZ     = 0b10100000,
        HardHiz     = 0b10101000,
        GetStatus   = 0b11010000
    };

    map<int, uint> microstepping_config {
        std::make_pair (1,   0b000),
        std::make_pair (2,   0b001),
        std::make_pair (4,   0b010),
        std::make_pair (8,   0b011),
        std::make_pair (16,  0b100),
        std::make_pair (32,  0b101),
        std::make_pair (64,  0b110),
        std::make_pair (128, 0b111),
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

    /**
     * @brief Decelerate motor from current speed, then set bridges to high impedance
     */
    void Soft_HiZ();

    /**
     * @brief initialize driver CONFIG register, configuration
     * 00011110
     * 10000000
     * - Hard stop at switch hit
     * - Stop at overcurrent detection
     */
    void Init();

public:

    /**
     * @brief   Set parametr MAX_SPEED in driver
     *          The register value is calculated according to the formula in datasheet page 43.
     *
     * @param max_speed     New maximal speed of motor
     */
    void Set_max_speed(uint max_speed);

    /**
     * @brief   Set parametr MIN_SPEED in driver
     *          The register value is calculated according to the formula in datasheet page 43.
     *
     * @param min_speed New minimal speed of motor
     */
    void Set_min_speed(uint min_speed);

    /**
     * @brief   Set parametr ACC in driver
     *          The register value is calculated according to the formula in datasheet page 42.
     *
     * @param acceleration  New acceleration of motor
     */
    void Set_acceleration(uint acceleration);

    /**
     * @brief   Set parametr DEC in driver
     *          The register value is calculated according to the formula in datasheet page 43.
     *
     * @param acceleration  New acceleration of motor
     */
    void Set_deceleration(uint deceleration);

    /**
     * @brief Calculate value for register SPEED
     *
     * @param speed Speed of motor in steps/s
     * @return uint Value for register
     */
    uint Calculate_speed(uint speed);

    /**
     * @brief   L6470 need to have every byte in transmittion divided by re-enabling
     *          chip select signal, this method will make transmition for every byte of data
     *
     * @param data  Data to send to L6470
     * @return int  Validity of action
     */
    int Send(vector<uint8_t> data);

    /**
     * @brief   Send one byte of data to driver
     *
     * @param data  Data to send to L6470
     * @return int  Validity of action
     */
    int Send(uint8_t data);

    /**
     * @brief Set given register of L6470 to value
     *
     * @param param Parameter to set
     * @param value New value for parameter
     * @param size  Size of target register in bits, value will be cropped to that size
     * @return int  Validity of action
     */
    int Set_param(register_map param, uint32_t value, uint size);

    /**
     * @brief   Return value of parametr from L6470
     *
     * @param param     Parameter to read
     * @param size      Size of target parameter
     * @return vector<uint8_t>  Value of parameter
     */
    vector<uint8_t> Get_param(register_map param, uint size);

    /**
     * @brief Set microsteping of driver
     *
     * @param microsteps    Number of microsteps, allowed values are:
     *                      1(full-step), 2(half-step), 4, 8, 16, 32, 64, 128
     *
     * @return int  validity of action, -1 if given microstepps cannot be set
     */
    int Set_microsteps(uint microsteps);

    /**
     * @brief   Run motor in given direction until hits switch
     *
     * @param dir   Direction where enstop is located
     */
    void GoHome(Direction dir);

    /**
     * @brief   Run motor with minimal speed in given directon until switch is released
     *
     *
     * @param dir Direction to move
     */
    void ReleaseSW(Direction dir);

};



