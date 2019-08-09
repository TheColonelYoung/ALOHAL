#include "i2c_master.hpp"

I2C_master::I2C_master(I2C_HandleTypeDef *handler, uint speed)
    : handler(handler), speed(speed)
{
}

uint I2C_master::Transmit_poll(uint8_t addr, vector<uint8_t> data)
{
    return HAL_I2C_Master_Transmit(handler, addr, (uint8_t *)data.data(), data.size(), 10);
}

vector<uint8_t> I2C_master::Receive_poll(uint8_t addr, uint length)
{
    vector<uint8_t> data(length);
    HAL_I2C_Master_Receive(handler, (uint8_t)addr, (uint8_t *)data.data(), length, 10);
    return data;
}

vector<uint8_t> I2C_master::Scan()
{
    //TODO
    return vector<uint8_t>();
}