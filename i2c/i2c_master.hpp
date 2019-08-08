/**
 * @file i2c.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

#include <vector>
#include <string>

#include "global_includes.hpp"

using namespace std;
typedef unsigned int uint;

class I2C_master
{
private:
    I2C_HandleTypeDef *handler;
    uint speed;
public:
    I2C_master() =default;
    I2C_master(I2C_HandleTypeDef *handler, uint speed = 100000);

    uint Send_poll(uint8_t addr, vector<uint8_t> data);
    vector<uint8_t> Receive_poll(uint8_t addr, uint length = 1);
};
