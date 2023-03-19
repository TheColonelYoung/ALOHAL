/**
 * @file spi_eeprom.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 28.09.2020
 */

#pragma once

#include <vector>
#include <string>

#include "spi/spi_device.hpp"
#include "gpio/pin.hpp"

typedef unsigned int uint;

/**
 * @brief   Class can be used for 8-bit SPI EEPROM
 *          Organization of memory up to 4 GB -> size of memory address offset are four bytes
 *          Only byte write and read is now supported, no page write/read
 *          Suitable and tested EEPROM: M95M02-DR M95M02-DF
 */
class SPI_EEPROM: public SPI_device
{
private:
    /**
     * @brief Size of memory address in bytes, usually 1-4
     *
     */
    unsigned short address_size = 2;

public:
    /**
     * @brief Construct a new i2c eeprom object
     *
     * @param master    Master SPI from mcu
     * @param address   Address of device
     */
    SPI_EEPROM(SPI_master &master, Pin *chip_select, short unsigned int address_size, bool cs_active = false);

    /**
     * @brief   Write one byte to selected address
     *
     * @param memory_offset Address in EEPROM to which will be writing performed
     * @param data          Byte to write
     * @return uint8_t      Status code of transmission
     */
    uint8_t Write(uint32_t memory_offset, uint8_t data);

    /**
     * @brief   Read one byte from selected address
     *
     * @param memory_offset Address in EEPROM from which will be reading performed
     * @return uint8_t      Data read from memory
     */
    uint8_t Read(uint32_t memory_offset);
};

