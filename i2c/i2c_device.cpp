#include "i2c_device.hpp"

I2C_device::I2C_device(I2C_master master, unsigned char address):
    master(master), address(address){

}

uint I2C_device::Transmit(vector<uint8_t> data){
    return master.Transmit_poll(address, data);
}

vector<uint8_t> I2C_device::Receive(uint length){
    return master.Receive_poll(address, length);
}

uint I2C_device::Write(vector<uint8_t> mem_address, vector<uint8_t> data){
    mem_address.insert( mem_address.end(), data.begin(), data.end() );
    return master.Transmit_poll(address, mem_address);
}

vector<uint8_t> I2C_device::Read(vector<uint8_t> mem_address, uint length){
    master.Transmit_poll(address, mem_address);
    return master.Receive_poll(address, length);
}

