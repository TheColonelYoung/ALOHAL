/**
 * @file device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.07.2019
 */
#pragma once

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L

#include "mcu/mcu.hpp"
#include "cli/cli.hpp"

using namespace std;

class Device{
private:

public:
    MCU mcu = MCU();
    CLI cli;

    Device() = default;

    void Init();
};

#include "gpio/pin.hpp"
