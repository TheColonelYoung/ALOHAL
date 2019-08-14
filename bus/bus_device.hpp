#pragma once

#include <vector>
#include <string>

using namespace std;
typedef unsigned int uint;

/**
 * @brief Generic class for every possible device on any bus
 * This allows devices on different bus as (I2C and SPI) to be managed by one system
 * Due to this cooperation of device and synchronous acces is easier
 */
class Bus_device
{
public:
    /**
     * @brief Construct a new Bus_device object
     */
    Bus_device() =default;

private:
    /**
     * @brief Function for generic transfer from master to slave on bus
     *
     * @param data Data to be send
     * @return uint Status of transmition
     */
    virtual uint Transmit(vector<uint8_t> data) = 0;

    /**
     * @brief Function for generic transfer from bus device to master
     *
     * @param length Number bytes to receive
     * @return vector<uint8_t> Received data
     */
    virtual vector<uint8_t> Receive(uint length) = 0;
};

