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
    #define INTERNAL_VOLTAGE_REFERENCE_ADDRESS ((uint16_t*)0x1fff75aa)
    #define INTERNAL_TEMPERATURE_CALIBRATION_1_ADDRESS ((uint16_t*)0x1fff75a8)
    #define INTERNAL_TEMPERATURE_CALIBRATION_2_ADDRESS ((uint16_t*)0x1fff75ca)
    #define INTERNAL_TEMPERATURE_REFERENCE_VOLTAGE (3.0f)
#endif
