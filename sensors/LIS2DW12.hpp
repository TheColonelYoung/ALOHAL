/**
 * @file LIS2DW12.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 02.11.2022
 */

#pragma once

using namespace std;

#include <vector>

#include "i2c/i2c_device.hpp"
#include "sensor.hpp"
#include "modifiers/loggable.hpp"

/**
 * @brief   LIS2DW12: MEMS digital output motion sensor - high-performance ultra-low-power 3-axis accelerometer
 *          Only basic functionality is implemented, (no interrupts fifo, fall/tap detection)
 */
class LIS2DW12 : public I2C_device, public Sensor, public Loggable
{
public:
    enum class Registers: uint8_t {
        OUT_T_L  = 0x0d,
        OUT_T_H  = 0x0e,
        WHO_AM_I = 0x0f,
        CTRL1    = 0x20,
        CTRL2    = 0x21,
        CTRL3    = 0x22,
        CTRL4_INT1_PAD_CTRL = 0x23,
        CTRL5_INT2_PAD_CTRL = 0x24,
        CTRL6   = 0x25,
        OUT_T   = 0x26,
        STATUS  = 0x27,
        OUT_X_L = 0x28,
        OUT_X_H = 0x29,
        OUT_Y_L = 0x2a,
        OUT_Y_H = 0x2b,
        OUT_Z_L = 0x2c,
        OUT_Z_H = 0x2d,
        CTRL7   = 0x3f
    };

    enum class Modes: uint8_t {
        Low_power         = 0b00, // Low-Power Mode (12/14-bit resolution)
        High_performance  = 0b01, // High-Performance Mode (14-bit resolution)
        Single_conversion = 0b10, // High-Performance Mode (14-bit resolution)
    };

    enum class Data_rate: uint8_t {
        Power_down      = 0b00,
        ON_12HP5_1LP6   = 0b0001, // High-Performance / Low-Power mode 12.5/1.6 Hz
        ON_12HP5        = 0b0010, // High-Performance / Low-Power mode 12.5 Hz
        ON_HP25         = 0b0011, // High-Performance / Low-Power mode 25 Hz
        ON_HP50         = 0b0100, // High-Performance / Low-Power mode 50 Hz
        ON_HP100        = 0b0101, // High-Performance / Low-Power mode 100 Hz
        ON_HP200        = 0b0110, // High-Performance / Low-Power mode 200 Hz
        ON_HP400_LP200  = 0b0111, // High-Performance / Low-Power mode 400/200 Hz
        ON_HP800_LP200  = 0b1000, // High-Performance / Low-Power mode 800/200 Hz
        ON_HP1600_LP200 = 0b1001, // High-Performance / Low-Power mode 1600/200 Hz
    };

    enum class Low_power_modes: uint8_t {
        LP_Mode_1 = 0b00, // Low-Power Mode 1 (12-bit resolution)
        LP_Mode_2 = 0b01, // Low-Power Mode 2 (14-bit resolution)
        LP_Mode_3 = 0b10, // Low-Power Mode 3 (14-bit resolution)
        LP_Mode_4 = 0b11, // Low-Power Mode 4 (14-bit resolution)
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL1{
        Low_power_modes LP_MODE : 2; // Low-power mode selection
        Modes MODE              : 2; // Mode selection
        Data_rate ODR           : 4; // Output data rate and mode selection
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL2{
        uint8_t SIM             : 1; // SPI serial interface mode selection. Default value: 0
        uint8_t I2C_DISABLE     : 1; // Disable I2C communication protocol. Default value: 0
        uint8_t IF_ADD_INC      : 1; // Register address automatically incremented, Default value: 1
        uint8_t BDU             : 1; // Block data update. Default value: 0
        uint8_t CS_PU_DISC      : 1; // Disconnect CS pull-up. Default value: 0
        uint8_t ALWAYS_ZERO     : 1; // This bit must be set to ‘0’ for the correct operation of the device
        uint8_t SOFT_RESET      : 1; // Soft reset acts as reset for all control registers, then goes to 0.
        uint8_t BOOT            : 1; // Boot enables retrieving the correct trimming parameters from nonvolatile memory  into registers where trimming parameters are stored.
    };

    enum class Self_test: uint8_t {
        Normal_mode     = 0b00,
        Positive_sign   = 0b01,
        Negative_sign   = 0b10,
        None            = 0b11
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL3{
        uint8_t SLP_MODE_1          : 1; // Single data conversion on demand mode enable
        uint8_t SLP_MODE_SEL        : 1; // Single data conversion on demand mode selection: 0 - INT2, 1 - I2C/SPI
        uint8_t ALWAYS_ZERO         : 1; // This bit must be set to ‘0’ for the correct operation of the device
        uint8_t H_LACTIVE           : 1; // Interrupt active high, low. Default value: 0
        uint8_t LIR                 : 1; // Latched Interrupt, Default value: 0
        uint8_t PP_OD               : 1; // Push-pull/open-drain selection on interrupt pad. Default value: 0
        Self_test ST2               : 2; // Self-test enable. Default value: 00
    };

    enum class Filtering_cutoff: uint8_t {
        ODR_2       = 0b00, // ODR/2 (up to ODR = 800 Hz, 400 Hz when ODR = 1600 Hz)
        ODR_4       = 0b01, // ODR/4 (HP/LP)
        ODR_10      = 0b10, // ODR/10 (HP/LP)
        ODR_20      = 0b11  // ODR/20 (HP/LP)
    };

    enum class Full_scale: uint8_t {
        G2      = 0b00, // ±2 g
        G4      = 0b01, // ±4 g
        G8      = 0b10, // ±8 g
        G12     = 0b11  // ±16 g
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL6{
        uint8_t ALWAYS_ZERO         : 2; // This bits must be set to ‘0’ for the correct operation of the device
        uint8_t LOW_NOISE           : 1; // Low-noise configuration
        uint8_t FDS                 : 1; // Filtered data type selection. Default value: 0
        Full_scale FS               : 2; // Full-scale selection
        Filtering_cutoff BW_FILT    : 2; // Bandwidth selection
    };

public:

        /**
         * @brief Construct a new LIS2DW12 object
         *
         * @param master
         * @param address   I2C address in 8-biz format (stuffed with 0 at the end)
         */
        LIS2DW12(I2C_master master, unsigned char address);

        /**
         * @brief   Reads ID from sensor register (0x0f)
         *
         * @return uint8_t  ID of sensor correct value is 0x44
         */
        uint8_t ID();

        /**
         * @brief Write value to a register
         *
         * @param register_name Name of register
         * @param value New value of register
         */
        uint Register(LIS2DW12::Registers register_name, uint8_t &value);

        /**
         * @brief Read value for register
         *
         * @param register_name Name of register
         * @return uint8_t
         */
        uint8_t Register(LIS2DW12::Registers register_name);

        /**
         * @brief Read acceleration values for all axis
         *
         * @return array<uint16_t, 3> [X,Y,Z] acceleration values
         */
        array<int16_t, 3> Acceleration();

    };
