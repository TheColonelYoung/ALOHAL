#include "i2c_eeprom.hpp"

I2C_EEPROM::I2C_EEPROM(I2C_master master, unsigned char address) :
    I2C_device(master, address)
{ }

uint8_t I2C_EEPROM::Write(uint16_t memory_offset, uint8_t data){
    return Transmit({ static_cast<uint8_t>(memory_offset >> 8),
                      static_cast<uint8_t>(memory_offset & 0xff),
                      data });
}

uint8_t I2C_EEPROM::Read(uint16_t memory_offset){
    Transmit({ static_cast<uint8_t>(memory_offset >> 8),
               static_cast<uint8_t>(memory_offset & 0xff) });
    return Receive(1)[0];
}
