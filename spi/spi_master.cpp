#include "spi_master.hpp"

SPI_master::SPI_master(SPI_HandleTypeDef *handler, uint speed)
    : handler(handler), speed(speed)
{
}

uint SPI_master::Transmit_poll(Pin chip_select, vector<uint8_t> data)
{
    uint status;
    chip_select.Set(0);
    status = HAL_SPI_Transmit(handler, (uint8_t *)data.data(), data.size(), 10);
    chip_select.Set(1);
    return status;
}

vector<uint8_t> SPI_master::Receive_poll(Pin chip_select, uint length)
{
    vector<uint8_t> data(length);
    chip_select.Set(0);
    HAL_SPI_Receive(handler, (uint8_t *)data.data(), length, 10);
    chip_select.Set(1);
    return data;
}
