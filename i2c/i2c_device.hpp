/**
 * @file i2c_device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

#include <vector>
#include <string>

#include "i2c/i2c_master.hpp"
typedef unsigned int uint;

/**
 * @brief Generic class for all devices, which are connected to I2C bus
 *
 */
class I2C_device
{
private:
    I2C_master master;
    uint8_t address = 0;

public:
    I2C_device() = default;
    /**
     * @brief Construct a new i2c device object
     *
     * @param master Object of master bus on which is device connected
     * @param address Address of device on bus
     */
    I2C_device(I2C_master master, uint8_t address);

    /**
     * @brief Transmit data to device
     *
     * @param data Data to be send by bus
     * @return uint Status code of transmittion
     */
    uint Transmit(vector<uint8_t> data);

    /**
     * @brief Receive data from device on bus
     *
     * @param length Number of bytes to be received
     * @return vector<uint8_t> received data
     */
    vector<uint8_t> Receive(uint length);
};