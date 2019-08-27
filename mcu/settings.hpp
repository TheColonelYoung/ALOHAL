/**
 * @file settings.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 15.08.2019
 */

#pragma once

// Add defines for family and model based on Makefile defined defines
#if defined(STM32L432xx)
    #define MCU_MODEL_NAME "STM32L432"
    #define MCU_FAMILY_STM32_L4
#elif defined(STM32L452xx)
    #define MCU_MODEL_NAME "STM32L452"
    #define MCU_FAMILY_STM32_L4
#endif

// Configuration of address if internal components
#if defined(STM32L432xx)
    // Only example not real address
    #define INTERNAL_TEMP_SENSOR_ADDRESS 0x00
#endif
