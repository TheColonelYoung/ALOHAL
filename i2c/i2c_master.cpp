#include "i2c_master.hpp"

I2C_master::I2C_master(I2C_HandleTypeDef *handler, uint speed)
    : handler(handler), speed(speed)
{
}

uint I2C_master::Transmit_poll(uint8_t addr, const vector<uint8_t> &data) const
{
    return HAL_I2C_Master_Transmit(handler, addr, (uint8_t *)data.data(), data.size(), 10);
}

vector<uint8_t> I2C_master::Receive_poll(uint8_t addr, uint length)
{
    vector<uint8_t> data(length);
    HAL_I2C_Master_Receive(handler, (uint8_t)addr, (uint8_t *)data.data(), length, 10);
    return data;
}

bool I2C_master::Ping(uint8_t addr){
    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(handler, (uint8_t)addr, 1, 1);

    if(status == HAL_OK){
        return true;
    } else {
        return false;
    }
}
