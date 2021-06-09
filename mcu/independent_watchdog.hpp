/**
 * @file independent_watchdog.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 07.06.2021
 */

#if   defined(MCU_FAMILY_STM32_F0)
# include "stm32f0xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_F1)
# include "stm32f1xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_F3)
# include "stm32f3xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_F4)
# include "stm32f4xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_F7)
# include "stm32f7xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_L0)
# include "stm32l0xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_L1)
# include "stm32l1xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_L4)
# include "stm32l4xx_hal_iwdg.h"
#elif defined(MCU_FAMILY_STM32_G0)
# include "stm32g0xx_hal_iwdg.h"
#endif // if   defined(MCU_FAMILY_STM32_F0)

#include <cstdint>

#include "global_includes.hpp"

extern IWDG_HandleTypeDef hiwdg;

#pragma once

/**
 * @brief   Watchdog is capable to reset mcu when is not periodically reset
 *          Is clocked from independent clock (LSI) - 32kHz, can be divided by prescaler
 *          Contains downcounting register, when this register reach zero, mcu is reset
 *          Also contains windows register, when counter is above value in window register and watchdog
 *              is reset, reset is considered as invalid and mcu is immediately reset
 *          Watchdog is disabled in debug mode by default.
 */
class Independent_watchdog {
    /**
     * @brief  HAL handler
     */
    IWDG_HandleTypeDef &handler = hiwdg;

public:

    /**
     * @brief Prescaler values, divides internal LSI oscillator
     */
    enum class Prescaler: uint8_t {
        _4   = 0,
        _8   = 1,
        _16  = 2,
        _32  = 3,
        _64  = 4,
        _128 = 5,
        _256 = 6
    };

public:

    /**
     * @brief
     *          Does not require HAL handler because there is only one watchdog
     */
    Independent_watchdog();

    /**
     * @brief Starts watchdog countdown
     *
     * @param prescaler Prescaler for internal oscillator
     * @param period    Value for reaload register from which will be watchdog counting down
     * @param window    Value for window register
     */
    void Start(Prescaler prescaler, uint16_t period_register, uint16_t window_register = 0);

    /**
     * @brief Resets watchdog counter register
     */
    void Reload();

    /**
     * @brief   Calculates value for watchdog registers from period given in ms
     *
     * @param period_ms     Period of watchdog reset or window
     * @param prescaler     Prescaler with which will be watchdog working
     * @param frequency     Frequency (Hz) of internal RC (LSI) oscillator which drives independent watchdog
     * @param counter_size  Size of counter register in bits
     * @return uint16_t     Value for watchdog register
     */
    static uint16_t Calculate_register(float period_ms, Prescaler prescaler, uint16_t frequency = 32000, uint8_t counter_size = 12);
};
