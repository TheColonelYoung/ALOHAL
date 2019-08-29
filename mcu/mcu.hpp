/**
 * @file mcu.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */

#pragma once
/*
#if __has_include("configuration/config.hpp")
    #include "configuration/config.hpp"
#else
    #include "configuration/config_default.hpp"
#endif*/

#include "configuration/config.cpp"
#include "mcu/configuration/settings.hpp"
#include "gpio/pin.hpp"

#include <string>

using namespace std;

/**
 * @brief Represent MCU(STM32) on board(device)
 *
 * Contains all configured hardware peripherals
 * Configuration of MCU is in file mcu/configuration/config.hpp
 * Initialization of MCU should be done from Device.Init as first
 * Configuration in ALOHAL must corresponds with CubeMX configuration
 */

class MCU
{
private:
    string model = MCU_MODEL_NAME;
public:
    MCU() =default;

    /**
     * @brief Initialize peripherals with their parameters
     * Should be called during device initialization
     *
     */
    void Init();

    // Peripherals

    // UART
    #ifdef UART_1_EN
    UART *UART_1;
    #endif
    #ifdef UART_2_EN
    UART *UART_2;
    #endif
    #ifdef UART_3_EN
    UART *UART_3;
    #endif
    #ifdef UART_4_EN
    UART *UART_4;
    #endif

    // I2C
    #ifdef I2C_1_EN
    I2C_master *I2C_1;
    #endif
    #ifdef I2C_2_EN
    I2C_master *I2C_2;
    #endif
    #ifdef I2C_3_EN
    I2C_master *I2C_3;
    #endif
    #ifdef I2C_4_EN
    I2C_master *I2C_4;
    #endif

    // SPI
    #ifdef SPI_1_EN
    SPI_master *SPI_1;
    #endif
    #ifdef SPI_2_EN
    SPI_master *SPI_2;
    #endif
    #ifdef SPI_3_EN
    SPI_master *SPI_3;
    #endif
    #ifdef SPI_4_EN
    SPI_master *SPI_4;
    #endif

    #ifdef TIM_1_EN
    Timer *TIM_1;
    #endif
    #ifdef TIM_2_EN
    Timer *TIM_2;
    #endif
    #ifdef TIM_3_EN
    Timer *TIM_3;
    #endif
    #ifdef TIM_4_EN
    Timer *TIM_4;
    #endif
    #ifdef TIM_5_EN
    Timer *TIM_5;
    #endif
    #ifdef TIM_6_EN
    Timer *TIM_6;
    #endif
    #ifdef TIM_7_EN
    Timer *TIM_7;
    #endif
    #ifdef TIM_8_EN
    Timer *TIM_8;
    #endif
    #ifdef TIM_9_EN
    Timer *TIM_9;
    #endif
    #ifdef TIM_10_EN
    Timer *TIM_10;
    #endif
    #ifdef TIM_11_EN
    Timer *TIM_11;
    #endif
    #ifdef TIM_12_EN
    Timer *TIM_12;
    #endif
    #ifdef TIM_13_EN
    Timer *TIM_13;
    #endif
    #ifdef TIM_14_EN
    Timer *TIM_14;
    #endif
    #ifdef TIM_15_EN
    Timer *TIM_15;
    #endif
    #ifdef TIM_16_EN
    Timer *TIM_16;
    #endif

};


