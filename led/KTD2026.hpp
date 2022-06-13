/**
 * @file KTD2026.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.06.2022
 */

#pragma once

#include "device/component.hpp"
#include "i2c/i2c_device.hpp"

using namespace std;

class KTD2026: public Component, public I2C_device{

public:
    /**
     * @brief Construct a new KTD2026 object
     *
     * @param master    Master I2C of MCU
     * @param address   Address of device
     */
    KTD2026(I2C_master master, unsigned char address);

    void Test();

    uint8_t Read();

private:
    /**
     * @brief   Address of used registers from BME280
     */
    enum class Register: uint8_t {
        EN_Reset            = 0x00,
        Flash_period        = 0x01,
        Flash_ontime1       = 0x02,
        Flash_ontime2       = 0x03,
        Channel_ctrl        = 0x04,
        Ramp_rate           = 0x05,
        LED1_out            = 0x06,
        LED2_out            = 0x07,
        LED3_out            = 0x08,
    };
};
