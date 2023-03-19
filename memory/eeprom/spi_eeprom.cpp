#include "spi_eeprom.hpp"

SPI_EEPROM::SPI_EEPROM(SPI_master &master, Pin *chip_select, short unsigned int address_size, bool cs_active) :
    SPI_device(master, chip_select, cs_active), address_size(address_size)
{
}

uint8_t SPI_EEPROM::Write(uint32_t memory_offset, uint8_t data){
    Transmit({static_cast<uint8_t>(0b00000110)});

    vector<uint8_t> transmission(address_size + 2);
    transmission[0] = 0b00000010;
    transmission[1] = memory_offset & 0x000000ff;
    if(address_size > 1){
        transmission[2] = (memory_offset & 0x0000ff00) >> 8;
    }
    if(address_size > 2){
        transmission[3] = (memory_offset & 0x00ff0000) >> 16;
    }
    if(address_size > 3){
        transmission[4] = (memory_offset & 0xff000000) >> 24;
    }
    transmission[address_size + 1] = data;
    return Transmit(transmission);
}

uint8_t SPI_EEPROM::Read(uint32_t memory_offset){
    vector<uint8_t> transmission(address_size+2);
    transmission[0] = 0b00000011;;
    transmission[1] = memory_offset & 0x000000ff;
    if(address_size > 1){
        transmission[2] = (memory_offset & 0x0000ff00) >> 8;
    }
    if(address_size > 2){
        transmission[3] = (memory_offset & 0x00ff0000) >> 16;
    }
    if(address_size > 3){
        transmission[4] = (memory_offset & 0xff000000) >> 24;
    }
    transmission[address_size + 1] = 0x00;
    return Transmit_and_Receive(transmission)[address_size + 1];
}
