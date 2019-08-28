/**
 * @file config.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 12.07.2019
 *
 * @brief   This is default configuration for MCU, serves only as example how it should look
 *          MCU configuration is specific for every project and should be customized
 *          Project specific ,config should be located in Src folder with name config.hpp
 *          To this file is pointing symbolic link from this folder
 *
 */
#pragma once

#warning "Default configuration from ALOHAL library is used, create project specific configuration"

/* External IRQ support
Enable this to support registring functions to external interupt event handler
*/

// #define EXT_IRQ_EN

// #define ADC_1_EN
// NOTE In higher families MCUs has more ADC, in this purpose conditional compile must be added

// #define DAC_1_EN

#define UART_1_EN
//#define UART_2_EN
// #define UART_3_EN
// #define UART_4_EN

// #define TIM_1_EN
#define TIM_2_EN
// #define TIM_3_EN
// #define TIM_4_EN
// #define TIM_5_EN
// #define TIM_6_EN
// #define TIM_7_EN
// #define TIM_8_EN
// #define TIM_9_EN
// #define TIM_10_EN
// #define TIM_11_EN
// #define TIM_12_EN
// #define TIM_13_EN
// #define TIM_14_EN
// #define TIM_15_EN
// #define TIM_16_EN

#define I2C_1_EN
//#define I2C_2_EN
//#define I2C_3_EN
//#define I2C_4_EN

//#define SPI_1_EN
//#define SPI_2_EN
//#define SPI_3_EN
//#define SPI_4_EN


// # define FLASH_EN

// #define OTS_EN
