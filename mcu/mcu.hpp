/**
 * @file mcu.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */

#pragma once

#include "globals.hpp"
#include "configuration/config.cpp"
#include "mcu/configuration/settings.hpp"
#include "mcu/configuration/externs.hpp"
#include "gpio/pin.hpp"
#include "timer/timer.hpp"

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
     * @brief   Perform initialization of mcu. This includes:
     *              - Peripherals
     */
    void Init();

    /**
     * @brief           Returns timefrom start of mcu
     *
     * @return float    Time from start of mcu in seconds
     */
    float Uptime();

    /**
     * @brief   Create files and folders inside filesystem
     *          Interface is depending on available peripherals
     *          This method is called during setting up of filesystem of device
     */
    void Filesystem_interface_initialization();

    /**
     * @brief Enable to configure timer frequency throught CLI
     *
     * @param timer Timer for CLI configuration
     */
    void Enable_CLI_control(Timer *timer);

    /**
     * @brief Enable to configure timer channel pulse throught CLI
     *
     * @param timer_channel Timer channel for CLI configuration
     */
    void Enable_CLI_control(TIM_channel *timer_channel);

    /**
     * @brief Initialize peripherals with their parameters
     * Should be called during device initialization
     */
    void Init_peripherals();



    // Peripherals

    IRQ_multi_handler<int> *EXT_IRQ = nullptr;

    #ifdef I_WDG_EN
    Independent_watchdog* I_WDG = new Independent_watchdog();
    #endif

    // ADC
    #ifdef ADC_1_EN
    AD_C *ADC_1 = nullptr;
    #endif
    #ifdef ADC_2_EN
    AD_C *ADC_2 = nullptr;
    #endif
    #ifdef ADC_3_EN
    AD_C *ADC_3 = nullptr;
    #endif

    #ifdef USB_CDC_EN
    USB_CDC *USB_port = nullptr;
    #endif

    // UART
    #ifdef UART_1_EN
    UART *UART_1 = nullptr;
    #endif
    #ifdef UART_2_EN
    UART *UART_2 = nullptr;
    #endif
    #ifdef UART_3_EN
    UART *UART_3 = nullptr;
    #endif
    #ifdef UART_4_EN
    UART *UART_4 = nullptr;
    #endif

    // I2C
    #ifdef I2C_1_EN
    I2C_master *I2C_1 = nullptr;
    #endif
    #ifdef I2C_2_EN
    I2C_master *I2C_2 = nullptr;
    #endif
    #ifdef I2C_3_EN
    I2C_master *I2C_3 = nullptr;
    #endif
    #ifdef I2C_4_EN
    I2C_master *I2C_4 = nullptr;
    #endif

    // SPI
    #ifdef SPI_1_EN
    SPI_master *SPI_1 = nullptr;
    #endif
    #ifdef SPI_2_EN
    SPI_master *SPI_2 = nullptr;
    #endif
    #ifdef SPI_3_EN
    SPI_master *SPI_3 = nullptr;
    #endif
    #ifdef SPI_4_EN
    SPI_master *SPI_4 = nullptr;
    #endif

    #ifdef TIM_1_EN
    Timer *TIM_1 = nullptr;
    #endif
    #ifdef TIM_2_EN
    Timer *TIM_2 = nullptr;
    #endif
    #ifdef TIM_3_EN
    Timer *TIM_3 = nullptr;
    #endif
    #ifdef TIM_4_EN
    Timer *TIM_4 = nullptr;
    #endif
    #ifdef TIM_5_EN
    Timer *TIM_5 = nullptr;
    #endif
    #ifdef TIM_6_EN
    Timer *TIM_6 = nullptr;
    #endif
    #ifdef TIM_7_EN
    Timer *TIM_7 = nullptr;
    #endif
    #ifdef TIM_8_EN
    Timer *TIM_8 = nullptr;
    #endif
    #ifdef TIM_9_EN
    Timer *TIM_9 = nullptr;
    #endif
    #ifdef TIM_10_EN
    Timer *TIM_10 = nullptr;
    #endif
    #ifdef TIM_11_EN
    Timer *TIM_11 = nullptr;
    #endif
    #ifdef TIM_12_EN
    Timer *TIM_12 = nullptr;
    #endif
    #ifdef TIM_13_EN
    Timer *TIM_13 = nullptr;
    #endif
    #ifdef TIM_14_EN
    Timer *TIM_14 = nullptr;
    #endif
    #ifdef TIM_15_EN
    Timer *TIM_15 = nullptr;
    #endif
    #ifdef TIM_16_EN
    Timer *TIM_16 = nullptr;
    #endif

};


