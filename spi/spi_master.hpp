/**
 * @file spi_master.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.08.2019
 */

#pragma once

#include <vector>
#include <string>

#include "global_includes.hpp"
#include "gpio/pin.hpp"

using namespace std;
typedef unsigned int uint;

/**
 * @brief SPI in master role, comunicates with other device connected to bus
 */
class SPI_master
{
private:
    SPI_HandleTypeDef *handler;
    uint speed;

public:
    /**
     * @brief Construct a new SPI master object
     */
    SPI_master() = default;

    /**
     * @brief Construct a new SPI master object
     *
     * @param handler Pointer to handler structure of SPI
     * @param speed Baudrate of bus
     */
    SPI_master(SPI_HandleTypeDef *handler, uint speed = 400000);

    /**
     * @brief Transmit data to device on bus in polling mode
     *
     * @param chip_select GPIO which serve as chip select of target device
     * @param data Data to be send
     * @param cs_active Logic level in which is chip select signal of target device enabled
     * @return uint Success = 0, Error - 1, Busy - 2, Timeout - 3
     */
    uint Transmit_poll(Pin &chip_select, const vector<uint8_t> &data, bool cs_active = false);

    /**
     * @brief Receive data from device on bus in polling mode
     *
     * @param chip_select
     * @param length Number of bytes received
     * @param cs_active Logic level in which is chip select signal of target device enabled
     * @return vector<uint8_t> Received data
     */
    vector<uint8_t> Receive_poll(Pin &chip_select, uint length = 1, bool cs_active = false);
};

