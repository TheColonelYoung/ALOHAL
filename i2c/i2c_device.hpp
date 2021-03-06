/**
 * @file i2c_device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

#include <vector>
#include <string>

#include "bus/bus_device.hpp"
#include "i2c/i2c_master.hpp"
typedef unsigned int uint;

/**
 * @brief Generic class for all devices, which are connected to I2C bus
 *
 */
class I2C_device: public Bus_device
{
private:
    /**
     * @brief Object of master bus on which is device connected
     */
    I2C_master master;

    /**
     * @brief   Address of device on I2C bus
     *          7 highest bits of byte, bit 0 is not used for addressing but for mode (read/write)
     *          0bxxxxxxx0, 7 bit address must be shifted to left
     */
    uint8_t address = 0;

public:
    I2C_device() = default;
    /**
     * @brief Construct a new I2C device object
     *
     * @param master Object of master I2C bus on which is device connected
     * @param address Address of device on I2C bus
     */
    I2C_device(I2C_master &master, unsigned char address);

protected:
    /**
     * @brief           Transmit data to device
     *
     * @param data      Data to be send by bus
     * @return uint     Status code of transmittion
     */
    uint Transmit(const vector<uint8_t> &data) const override;

    /**
     * @brief Receive data from device on bus
     *
     * @param length            Number of bytes to be received
     * @return vector<uint8_t>  Received data
     */
    vector<uint8_t> Receive(uint length)  override;

    /**
     * @brief   Write data to device using standart I2C method
     *          Sending address of device register and then data to write
     *          Address is not autoincremented
     *
     * @param mem_address   Address in device memory to write data
     * @param data          Data to be written into device
     * @return uint         Status code of transmittion
     */
    uint Write(uint8_t mem_address, vector<uint8_t> &data);

    /**
     * @brief   Read data from device memory
     *          Address is transmitted to device and then is received number of bytes from device
     *
     * @param mem_address       Address in device memory to read data
     * @param length            Number of bytes to be received
     * @return vector<uint8_t>  Received data
     */
    vector<uint8_t> Read(uint8_t mem_address, uint length);
};
