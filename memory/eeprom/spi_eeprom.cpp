#include "spi_eeprom.hpp"

SPI_EEPROM::SPI_EEPROM(SPI_HandleTypeDef *handler, Pin *chip_select, short unsigned int address_size, bool cs_active) :
    handler(handler), chip_select(chip_select), address_size(address_size), cs_active(cs_active)
{
}

uint8_t SPI_EEPROM::Write(uint32_t memory_offset, uint8_t data){
    //Transmit({static_cast<uint8_t>(0b00000110)});

    chip_select->Set(cs_active);
    HAL_SPI_Transmit(handler, &WREN, 1, 10);
    chip_select->Set(!cs_active);

    vector<uint8_t> transmission(address_size + 2);
    transmission[0] = 0b00000010;
    transmission[3] = memory_offset & 0x000000ff;
    transmission[2] = (memory_offset & 0x0000ff00) >> 8;
    transmission[1] = (memory_offset & 0x00ff0000) >> 16;
    transmission[address_size + 1] = data;

    chip_select->Set(cs_active);
    HAL_SPI_Transmit(handler, (uint8_t *)transmission.data(), transmission.size(), 10);
    chip_select->Set(!cs_active);

    return 0;//Transmit(transmission);
}

uint8_t SPI_EEPROM::Read(uint32_t memory_offset){
    vector<uint8_t> transmission(address_size+2);
    transmission[0] = 0b00000011;
    transmission[3] = memory_offset & 0x000000ff;
    transmission[2] = (memory_offset & 0x0000ff00) >> 8;
    transmission[1] = (memory_offset & 0x00ff0000) >> 16;
    transmission[address_size + 1] = 0x00;

    array<uint8_t,5> received_data;

    chip_select->Set(cs_active);
    HAL_SPI_TransmitReceive(handler, (uint8_t *)transmission.data(), (uint8_t *)received_data.data(), transmission.size(), 10);
    chip_select->Set(!cs_active);

    return received_data[address_size + 1];//Transmit_and_Receive(transmission)[address_size + 1];
}
