/**
 * @file spi_eeprom.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 28.09.2020
 */

#pragma once

#include <vector>
#include <array>
#include <string>

#include "global_includes.hpp"
#include "spi/spi_device.hpp"
#include "gpio/pin.hpp"

typedef unsigned int uint;

/**
 * @brief   Class can be used for 8-bit SPI EEPROM
 *          Organization of memory up to 4 GB -> size of memory address offset are four bytes
 *          Only byte write and read is now supported, no page write/read
 *          Suitable and tested EEPROM: M95M02-DR M95M02-DF
 */
class SPI_EEPROM{
private:

    SPI_HandleTypeDef *handler;

    Pin *chip_select = nullptr;

    /**
     * @brief Size of memory address in bytes, usually 1-4
     *
     */
    unsigned short address_size = 2;

    bool cs_active;

    uint8_t WREN = 0b00000110;

public:
    /**
     * @brief Construct a new i2c eeprom object
     *
     * @param master    Master SPI from mcu
     * @param address   Address of device
     */
    SPI_EEPROM(SPI_HandleTypeDef *handler, Pin *chip_select, short unsigned int address_size, bool cs_active = false);

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

    template <size_t size = 1>
    uint8_t Read_array(uint32_t start_address, std::array<uint8_t, size> &data) const {
        std::vector<uint8_t> transmission(address_size + 1);
        transmission[0] = 0b00000011;
        transmission[3] = start_address & 0x000000ff;
        transmission[2] = (start_address & 0x0000ff00) >> 8;
        transmission[1] = (start_address & 0x00ff0000) >> 16;

        chip_select->Set(cs_active);
        HAL_SPI_Transmit(handler, (uint8_t *)transmission.data(), transmission.size(), 10);
        HAL_SPI_Receive(handler, data.data(), data.size(), 10);
        chip_select->Set(!cs_active);
        return data.size();
    }

    template <size_t size = 1>
    void Write_array(uint32_t start_address, std::array<uint8_t, size> &data){

        chip_select->Set(cs_active);
        HAL_SPI_Transmit(handler, &WREN, 1, 10);
        chip_select->Set(!cs_active);

        std::vector<uint8_t> transmission(address_size + 1);
        transmission[0] = 0b00000010;
        transmission[3] = start_address & 0x000000ff;
        transmission[2] = (start_address & 0x0000ff00) >> 8;
        transmission[1] = (start_address & 0x00ff0000) >> 16;

        chip_select->Set(cs_active);
        HAL_SPI_Transmit(handler, (uint8_t *)transmission.data(), transmission.size(), 100);
        HAL_SPI_Transmit(handler, data.data(), data.size(), 100);
        chip_select->Set(!cs_active);
        return;
    }
};

