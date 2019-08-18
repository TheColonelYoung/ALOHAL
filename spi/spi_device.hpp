/**
 * @file spi_device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.08.2019
 */


#pragma once

#include <vector>
#include <string>

#include "bus/bus_device.hpp"
#include "spi/spi_master.hpp"
#include "gpio/pin.hpp"
typedef unsigned int uint;

/**
 * @brief Generic class for all devices, which are connected to SPI bus
 *
 */
class SPI_device: public Bus_device
{
private:
    SPI_master master;

    /**
     * @brief   GPIO of MCU or expander to which has device connected chip_select(slave select)
     *          Pointer to Pin must be used due to polymorphism,
     *               then can be used class derivated from Pin, as PinMCP23017
     */
    Pin *chip_select;

    /**
     * @brief Logic level of cip select signal in which is device active
     */
    bool cs_active;

public:
    SPI_device() = default;

    /**
     * @brief Construct a new spi device object
     *
     * @param master Object of master bus on which is device connected
     * @param chip_select Pin to which is connected chip_select input of device
     * @param cs_active Logic level of chip_select in which is device active
     */
    SPI_device(SPI_master master, Pin *chip_select, bool cs_active = false);

    /**
     * @brief Transmit data to device
     *
     * @param data Data to be send by bus
     * @return uint Status code of transmittion
     */
    uint Transmit(vector<uint8_t> data) override;

    /**
     * @brief Receive data from device on bus
     *
     * @param length Number of bytes to be received
     * @return vector<uint8_t> received data
     */
    vector<uint8_t> Receive(uint length)  override;
};
