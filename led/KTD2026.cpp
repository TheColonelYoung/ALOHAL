#include "KTD2026.hpp"

KTD2026::KTD2026(I2C_master master, unsigned char address) :
    Component("KTD2026"),
    I2C_device(master, address)
{ }

void KTD2026::Test(){
    Transmit({static_cast<uint8_t>(Register::EN_Reset),0b00000111});
    Transmit({static_cast<uint8_t>(Register::EN_Reset),0b00011000});
    Transmit({static_cast<uint8_t>(Register::Channel_ctrl),0b00010000});
    Transmit({static_cast<uint8_t>(Register::LED1_out),0x00});
    Transmit({static_cast<uint8_t>(Register::LED2_out),0x00});
    Transmit({static_cast<uint8_t>(Register::LED3_out),0x10});
}

uint8_t KTD2026::Read(){
    Transmit({static_cast<uint8_t>(Register::EN_Reset)});
    return Receive(1)[0];
}
