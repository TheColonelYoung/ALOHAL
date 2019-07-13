/**
 * @file mcu.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */

#pragma once

#include "mcu/config.hpp"

#include "gpio/pin.hpp"

#if defined(UART_1_EN) || defined(UART_2_EN) || defined(UART_3_EN) || defined(UART_4_EN)
    #include "uart/uart.hpp"
#endif

#include <string>

using namespace std;

class MCU
{
private:
    string model = MCU_MODEL;
public:
    MCU() =default;

    void Init();

    // Peripherals

    // UART
    #ifdef UART_1_EN
    UART UART_1;
    #endif
    #ifdef UART_2_EN
    UART UART_2;
    #endif
    #ifdef UART_3_EN
    UART UART_3;
    #endif
    #ifdef UART_4_EN
    UART UART_4;
    #endif
};


