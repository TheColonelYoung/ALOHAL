/**
 * @file HTS221.hpp
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
 * @brief   HTS221: Digital output magnetic sensor
 */
class HTS221 : public I2C_device, public Sensor, public Loggable
{
private:
    double temperature_zero = 0;
    double temperature_slope = 0;
    double humidity_zero = 0;
    double humidity_slope = 0;

public:
    enum class Registers: uint8_t {
        WHO_AM_I   = 0x0f,
        AV_CONF    = 0x10,
        CTRL1      = 0x20,
        CTRL2      = 0x21,
        CTRL3      = 0x22,
        STATUS     = 0x27,
        HUM_OUT_L  = 0x28,
        HUM_OUT_H  = 0x29,
        TEMP_OUT_L = 0x2a,
        TEMP_OUT_H = 0x2b,

        CALIB_H0_rH_x2      = 0x30,
        CALIB_H1_rH_x2      = 0x31,
        CALIB_T0_degC_x8    = 0x32,
        CALIB_T1_degC_x8    = 0x33,
        CALIB_T1_T0_MSB     = 0x35,
        CALIB_H0_T0_OUT_L   = 0x36,
        CALIB_H0_T0_OUT_H   = 0x37,
        CALIB_H1_T0_OUT_L   = 0x3a,
        CALIB_H1_T0_OUT_H   = 0x3b,
        CALIB_T0_OUT_L      = 0x3c,
        CALIB_T0_OUT_H      = 0x3d,
        CALIB_T1_OUT_L      = 0x3e,
        CALIB_T1_OUT_H      = 0x3f,
    };

    enum class Temperature_average: uint8_t {
        AVGT_2      = 0b000,
        AVGT_4      = 0b001,
        AVGT_8      = 0b010,
        AVGT_16     = 0b011,
        AVGT_32     = 0b100,
        AVGT_64     = 0b101,
        AVGT_128    = 0b110,
        AVGT_256    = 0b111,
    };

    enum class Humidity_average: uint8_t {
        AVGH_4      = 0b000,
        AVGH_8      = 0b001,
        AVGH_16     = 0b010,
        AVGH_32     = 0b011,
        AVGH_64     = 0b100,
        AVGH_128    = 0b101,
        AVGH_256    = 0b110,
        AVGH_521    = 0b111,
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) AV_CONF{
        Humidity_average AVGH       : 3; // Humidity average configuration
        Temperature_average AVGT    : 3; // Temperature average configuration
        uint8_t Reserved            : 2; // Reserved
    };

    enum class Output_data_rate: uint8_t {
        One_shot    = 0b00, // One measurement
        TH_1Hz      = 0b01, // 1 Hz output data rate
        TH_7Hz      = 0b10, // 7 Hz output data rate
        TH_12Hz5    = 0b11  // 12.5 Hz output data rate
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL1{
        Output_data_rate ODR        : 2; // Output data rate selection
        uint8_t BDU                 : 1; // Block data update, 0 - continuous, 1 - not update until reading MSB and LSB
        uint8_t Reserved            : 4; // Reserved
        uint8_t PD                  : 1; // Power down
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL2{
        uint8_t OneShot             : 1; // One Shot enable
        uint8_t Heater              : 1; // Enable internal heater
        uint8_t Reserved            : 5; // Reserved
        uint8_t BOOT                : 1; // Reboot memory content
    };

    struct __attribute__((packed)) __attribute__((__may_alias__)) CTRL3{
        uint8_t Reserved1           : 2; // Reserved
        uint8_t DRDY                : 1; // Data Ready enable
        uint8_t Reserved2           : 3; // Reserved
        uint8_t PP_OD               : 1; // Push-pull / Open Drain selection on pin 3 (DRDY)
        uint8_t DRDY_H_L            : 1; // Data Ready output signal active high, low
    };

    /**
     * @brief Construct a new HTS221 object
     *
     * @param master
     * @param address   I2C address in 8-bit format (stuffed with 0 at the end)
     */
    HTS221(I2C_master master, unsigned char address = 0xbe);

    /**
     * @brief   Reads ID from sensor register (0x0f)
     *
     * @return uint8_t  ID of sensor correct value is 0xbc
     */
    uint8_t ID();

    /**
     * @brief Write value to a register
     *
     * @param register_name Name of register
     * @param value New value of register
     */
    uint Register(HTS221::Registers register_name, uint8_t & value);

    /**
     * @brief Read value for register
     *
     * @param register_name Name of register
     * @return uint8_t
     */
    uint8_t Register(HTS221::Registers register_name);

    float Temperature();

    float Humidity();

    void Calibrate();

};
