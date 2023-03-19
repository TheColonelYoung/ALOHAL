/**
 * @file LIS3MDL.hpp
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
 * @brief   LIS3MDL: Digital output magnetic sensor
 */
class LIS3MDL : public I2C_device, public Sensor, public Loggable
{
public:
    enum class Registers: uint8_t {
        WHO_AM_I   = 0x0f,
        CTRL1      = 0x20,
        CTRL2      = 0x21,
        CTRL3      = 0x22,
        CTRL4      = 0x23,
        CTRL5      = 0x24,
        STATUS     = 0x27,
        OUT_X_L    = 0x28,
        OUT_X_H    = 0x29,
        OUT_Y_L    = 0x2a,
        OUT_Y_H    = 0x2b,
        OUT_Z_L    = 0x2c,
        OUT_Z_H    = 0x2d,
        TEMP_OUT_L = 0x2e,
        TEMP_OUT_H = 0x2f,
        INT_CFG    = 0x30,
        INT_SRC    = 0x31,
        INT_THS_L  = 0x32,
        INT_THS_H  = 0x33
    };

    enum class Data_rate: uint8_t {
        ODR_0_65 = 0b000,
        ODR_1_25 = 0b001,
        ODR_2_5  = 0b010,
        ODR_5    = 0b011,
        ODR_10   = 0b100,
        ODR_20   = 0b101,
        ODR_40   = 0b110,
        ODR_80   = 0b111,
    };

    enum class Axis_mode: uint8_t {
        LP  = 0b00, // Low-power mode
        MP  = 0b01, // Medium-performance mode
        HP  = 0b10, // High-performance mode
        UHP = 0b11, // Ultra-high-performance mode
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL1{
        uint8_t SELF_TEST           : 1; // Self-test
        uint8_t FAST_ODR            : 1; // Enables data rates above 80Hz
        Data_rate ODR               : 3; // Output data rate selection
        Axis_mode OM                : 2; // X and Y axes operative mode selection
        uint8_t TEMP_EN             : 1; // Temperature sensor enable
    };

    enum class Full_scale: uint8_t {
        G4  = 0b00, // ±4 gauss
        G8  = 0b01, // ±8 gauss
        G12 = 0b10, // ±12 gauss
        G16 = 0b11  // ±16 gauss
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL2{
        uint8_t ALWAYS_ZERO         : 2; // This bits must be set to ‘0’ for the correct operation of the device
        uint8_t SOFT_RST            : 1; // Configuration registers and user register reset function.
        uint8_t REBOOT              : 1; // Reboot memory content
        uint8_t ZERO                : 1; // This bit must be set to ‘0’ for the correct operation of the device
        Full_scale FS               : 2; // Full-scale configuration
    };

    enum class System_operation_mode: uint8_t {
        Continuous_conversion = 0b00, // Continuous-conversion mode
        Single_conversion     = 0b01, // Single-conversion mode has to be used with sampling frequency from 0.625 Hz to 80Hz.
        Power_down            = 0b10, // Power-down mode
        Power_down2            = 0b11, // Power-down mode
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL3{
        System_operation_mode MD    : 2; // Operating mode selection
        uint8_t SIM                 : 1; // SPI serial interface mode selection
        uint8_t ALWAYS_ZERO         : 2; // This bits must be set to ‘0’ for the correct operation of the device
        uint8_t LP                  : 1; // Low-power mode configuration
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL4{
        uint8_t ALWAYS_ZERO         : 1; // This bit must be set to ‘0’ for the correct operation of the device
        uint8_t BLE                 : 1; // Big/Little Endian data selection, 0: data LSb at lower address
        Axis_mode OMZ               : 2; // Z axis operative mode selection
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL5{
        uint8_t ALWAYS_ZERO         : 6; // This bits must be set to ‘0’ for the correct operation of the device
        uint8_t BDU                 : 1; // FAST READ allows reading the high part of DATA OUT only in order
        uint8_t FAST_READ           : 1; // Block data update for magnetic data, 0: continuous update
    };

    /**
     * @brief Construct a new LIS3MDL object
     *
     * @param master
     * @param address   I2C address in 8-bit format (stuffed with 0 at the end)
     */
    LIS3MDL(I2C_master master, unsigned char address);

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
    uint Register(LIS3MDL::Registers register_name, uint8_t & value);

    /**
     * @brief Read value for register
     *
     * @param register_name Name of register
     * @return uint8_t
     */
    uint8_t Register(LIS3MDL::Registers register_name);


    array<int16_t, 3> Magnetic_field_intensity();

};
