/**
 * @file mcu.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */

#pragma once

#include "mcu/mcu_config/mcu_names.hpp"
#include <string>

class MCU
{
private:
    std::string model;
public:
    MCU() =default;

    void Init();
};

