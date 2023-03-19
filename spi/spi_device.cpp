#include "spi/spi_device.hpp"

SPI_device::SPI_device(SPI_master &master, Pin *chip_select, bool cs_active):
    master(master), chip_select(chip_select), cs_active(cs_active){}

uint SPI_device::Transmit(const vector<uint8_t> &data) const{
    return master.Transmit_poll(*chip_select, data, cs_active);
}

vector<uint8_t> SPI_device::Receive(uint length){
    return master.Receive_poll(*chip_select, length, cs_active);
}

vector<uint8_t> SPI_device::Transmit_and_Receive(const vector<uint8_t> &data){
    return master.Transmit_and_Receive_poll(*chip_select, data, cs_active);
}
