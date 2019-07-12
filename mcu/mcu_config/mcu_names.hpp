/**
 * @file mcu_names.h
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */

#pragma once

#ifdef MCU_MODEL
    #undef MCU_MODEL
#endif

#ifdef STM32L432xx
    #define MCU_MODEL "STM32L432"
    #include "STM32L432.hpp"
#endif

#ifdef MCU_MODEL
    #pragma message( "Selected procesor model: " MCU_MODEL)
#else
    #error "MCU is not selected"
#endif