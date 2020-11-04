/**
 * @file BME280.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 28.10.2020
 */

#pragma once

#include <map>

#include "i2c/i2c_device.hpp"
#include "sensor.hpp"
#include "modifiers/loggable.hpp"

using namespace std;

/**
 * @brief
 *
 *
 *          In non-volatile memory contains read-only calibration data
 */
class BME280 : public I2C_device, public Sensor, public Loggable
{
private:
    /**
     * @brief   Address of used registers from BME280
     */
    enum class Register: uint8_t {
        DIG_T1           = 0x88,

        DIG_P1           = 0x8e,

        DIG_H1           = 0xa1,
        DIG_H2           = 0xe1,
        DIG_H3           = 0xe3,
        DIG_H4           = 0xe4,
        DIG_H5           = 0xe5,
        DIG_H6           = 0xe7,

        ChipID           = 0xd0,
        Reset            = 0xe0,

        CAL26            = 0xe1,

        ControlHumid     = 0xf2,
        Status           = 0Xf3,
        ControlMeas      = 0xf4,
        Config           = 0xf5,

        Temperature_data = 0xfa,
        Pressure_data    = 0xf7,
        Humidity_data    = 0xfd
    };

    /**
     * @brief   Modes of sensor and constants to write to ControlMeas register to enable mode
     */
    enum class Modes: uint8_t {
        Idle   = 0b00,
        Forced = 0b01,
        Normal = 0b11,
    };

    /**
     * @brief   Defines if data in calibration vector are valid
     */
    bool calibration_data_loaded = false;

    int32_t last_known_temperature = 0;

    /**
     * @brief   Temperature calibration data read from sensor
     */
    vector<std::uint16_t> temperature_calibration;

    /**
     * @brief   Presure calibration data read from sensor
     */
    vector<std::uint16_t> pressure_calibration;

    /**
     * @brief   Humidity calibration data read from sensor
     */
    vector<std::uint16_t> humidity_calibration;

public:
    BME280(I2C_master master, unsigned char address);

    /**
     * @brief   Reads ID from sensor register (0xd0)
     *
     * @return uint8_t  ID of sensor correct value is 0x60
     */
    uint8_t ID();

    /**
     * @brief   Checks if sensor is performing a measuring
     *
     * @return true     Measuring if performing now
     * @return false    Sensor is not measuring
     */
    bool Measuring();

    /**
     * @brief   Data from non-volatile memory are copied after start of sensor to regsiters
     *          This status is signaled by 0. bit in status register(STATUS)
     *
     * @return true     Data are still copied
     * @return false    Data are ready in registers
     */
    bool Reading_calibration();

    /**
     * @brief   Perform soft reset of sensor
     *          Writes value 0xb6 to reset register (0xE0), only 0xb6 will perform a reset, others are ignore
     */
    void Reset();

    /**
     * @brief   Reads temperature from sensor
     *
     * @return double   Temperature in celsius
     */
    double Temperature();

    /**
     * @brief   Set sensor to mode of continuous measurements
     */
    void Enter_normal_mode();

    /**
     * @brief   Set sensor to idle mode
     */
    void Enter_idle_mode();

    /**
     * @brief   Force one cycle of measuring
     *          Measuring is not performed immediately
     */
    void Force_measurement();

private:
    /**
     * @brief       Loads calibration data from sensor to mcu
     *
     * @return true     Data loading is done
     * @return false    Data are loaded
     */
    bool Load_calibration_data();

    /**
     * @brief Enter selected mode
     *
     * @param mode  Mode to enter
     */
    void Enter_mode(Modes mode);

    /**
     * @brief Read ADC value from sensor
     *
     * @param ADC_register      ADC output registr to read
     * @return std::uint32_t    ADC value
     */
    std::uint32_t Load_ADC(Register ADC_register);
};
