/**
 * @file externs.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief This file externs Handlers from HAL to ALOHAL
 * @version 0.1
 * @date 03.09.2019
 */

#pragma once

#include "global_includes.hpp"

// FLASH
class Flash_mem;
#ifdef FLASH_EN
# include "flash/flash.hpp"
extern Flash_mem Flash;
#endif

// ADC
class AD_C;
#if defined(ADC_1_EN) || defined(ADC_2_EN) || defined(ADC_3_EN)
# include "adc/adc.hpp"
#endif


#ifdef ADC_1_EN
extern ADC_HandleTypeDef hadc1;
#endif
#ifdef ADC_2_EN
extern ADC_HandleTypeDef hadc2;
#endif
#ifdef ADC_3_EN
extern ADC_HandleTypeDef hadc3;
#endif

// DAC
class DA_C;
#ifdef DAC_1_EN
# include "dac/DAC.hpp"
extern DAC_HandleTypeDef hdac;
extern DA_C DAC_1;
#endif

class USB_CDC;
#ifdef USB_CDC_EN
# include "usb/cdc_vcp.hpp"
extern USB_CDC USB_port;
#endif

class UART;
#if defined(UART_1_EN) || defined(UART_2_EN) || defined(UART_1_EN) || defined(UART_3_EN)
# include "uart/uart.hpp"
#endif

#ifdef UART_1_EN
extern UART_HandleTypeDef huart1;
#endif
#ifdef UART_2_EN
extern UART_HandleTypeDef huart2;
#endif
#ifdef UART_3_EN
extern UART_HandleTypeDef huart3;
#endif
#ifdef UART_4_EN
extern UART_HandleTypeDef huart4;
#endif

// TIMERS
class Timer;

#define ALOHAL_CREATE_TIMER(name, handler, size, channels) \
    name = new Timer(&handler, size, channels); \
    ALOHAL_TIM_CHAN_BACKPOINTER(name)

#define ALOHAL_TIM_CHAN_BACKPOINTER(timer) \
    for (uint i = 0; i < timer->channel.size(); i++) { \
        timer->channel[i]._parent_timer = timer; }

#ifdef TIM_1_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim1;
extern Timer TIM_1;
#endif

#ifdef TIM_2_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim2;
extern Timer TIM_2;
#endif

#ifdef TIM_3_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim3;
extern Timer TIM_3;
#endif

#ifdef TIM_4_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim4;
extern Timer TIM_4;
#endif

#ifdef TIM_5_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim5;
extern Timer TIM_5;
#endif

#ifdef TIM_6_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim6;
extern Timer TIM_6;
#endif

#ifdef TIM_7_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim7;
extern Timer TIM_7;
#endif

#ifdef TIM_8_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim8;
extern Timer TIM_8;
#endif

#ifdef TIM_9_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim9;
extern Timer TIM_9;
#endif

#ifdef TIM_10_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim10;
extern Timer TIM_10;
#endif

#ifdef TIM_11_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim11;
extern Timer TIM_11;
#endif

#ifdef TIM_12_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim12;
extern Timer TIM_12;
#endif

#ifdef TIM_13_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim13;
extern Timer TIM_13;
#endif

#ifdef TIM_14_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim14;
extern Timer TIM_14;
#endif

#ifdef TIM_15_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim15;
extern Timer TIM_15;
#endif

#ifdef TIM_16_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim16;
extern Timer TIM_16;
#endif



class I2C_master;
#if defined(I2C_1_EN) || defined(I2C_2_EN) || defined(I2C_3_EN) || defined(I2C_4_EN)
# include "i2c/i2c_master.hpp"
#endif

#ifdef I2C_1_EN
extern I2C_HandleTypeDef hi2c1;
#endif
#ifdef I2C_2_EN
extern I2C_HandleTypeDef hi2c2;
#endif
#ifdef I2C_3_EN
extern I2C_HandleTypeDef hi2c3;
#endif
#ifdef I2C_4_EN
extern I2C_HandleTypeDef hi2c4;
#endif

class SPI_master;
#if defined(SPI_1_EN) || defined(SPI_2_EN) || defined(SPI_3_EN) || defined(SPI_4_EN)
# include "spi/spi_master.hpp"
#endif

#ifdef SPI_1_EN
extern SPI_HandleTypeDef hspi1;
#endif
#ifdef SPI_2_EN
extern SPI_HandleTypeDef hspi2;
#endif
#ifdef SPI_3_EN
extern SPI_HandleTypeDef hspi3;
#endif
#ifdef SPI_4_EN
extern SPI_HandleTypeDef hspi4;
#endif
