/**
 * @file i2c_eeprom.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 28.09.2020
 */

#pragma once

#include <vector>
#include <string>

#include "i2c/i2c_device.hpp"

typedef unsigned int uint;

/**
 * @brief   Class can be used for 8-bit I2C EEPROM
 *          Organization of memory 4x8b up to 65536x8b -> size of memory address offset are two bytes
 *          Only byte write and read is now supported, no page write/read
 *          Suitable and tested EEPROM: M24C32-FMC6TG, M24128-BFMC6TG
 */
class I2C_EEPROM: public I2C_device
{
public:
    /**
     * @brief Construct a new i2c eeprom object
     *
     * @param master    Master I2C from mcu
     * @param address   Address of device
     */
    I2C_EEPROM(I2C_master master, unsigned char address);

    /**
     * @brief   Write one byte to selected address
     *
     * @param memory_offset Address in EEPROM to which will be writing performed
     * @param data          Byte to write
     * @return uint8_t      Status code of transmittion
     */
    uint8_t Write(uint16_t memory_offset, uint8_t data);


    /**
     * @brief   Read one byte from selected address
     *
     * @param memory_offset Address in EEPROM from which will be reading performed
     * @return uint8_t      Data read from memory
     */
    uint8_t Read(uint16_t memory_offset);
};

