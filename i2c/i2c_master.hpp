/**
 * @file i2c.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

#include <vector>
#include <string>

#include "global_includes.hpp"

using namespace std;
typedef unsigned int uint;

/**
 * @brief I2C Bus in master role, comunicates with other device connected to bus
 */
class I2C_master
{
private:
    I2C_HandleTypeDef *handler;
    uint speed;

public:
    /**
     * @brief Construct a new i2c master object
     */
    I2C_master() = default;

    /**
     * @brief Construct a new i2c master object
     *
     * @param handler pointer to handler structure of I2C
     * @param speed baudrate of bus
     */
    I2C_master(I2C_HandleTypeDef *handler, uint speed = 100000);

    /**
     * @brief Transmit data to device on bus in polling mode
     *
     * @param addr Target device address
     * @param data Data to be send
     * @return uint Success = 0, Error - 1, Busy - 2, Timeout - 3
     */
    uint Transmit_poll(uint8_t addr, const vector<uint8_t> &data) const;

    /**
     * @brief Receive data from device on bus in polling mode
     *
     * @param addr Address of target device
     * @param length Number of bytes received
     * @return vector<uint8_t> Received data
     */
    vector<uint8_t> Receive_poll(uint8_t addr, uint length = 1);

    /**
     * @brief Test if target device is responding with ACK
     *
     * @param addr      Address of target device
     * @return true     Device is present and ready (sending ACK)
     * @return false    Device is not responding with ACKs
     */
    bool Ping(uint8_t addr);
};
