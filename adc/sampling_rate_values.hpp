/**
 * @file sampling_rate_values.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief
 * @version 0.1
 * @date 17.08.2020
 */
#pragma once

enum class Sampling_rate {
    #ifdef ADC_SAMPLETIME_1CYCLE_5
        _1C5,
    #endif

    #ifdef ADC_SAMPLETIME_2CYCLES_5
        _2C5,
    #endif

    #ifdef ADC_SAMPLETIME_3CYCLES_5
        _3C5,
    #endif

    #ifdef ADC_SAMPLETIME_6CYCLES_5
        _6C5,
    #endif

    #ifdef ADC_SAMPLETIME_7CYCLES_5
        _7C5,
    #endif

    #ifdef ADC_SAMPLETIME_12CYCLES_5
        _12C5,
    #endif

    #ifdef ADC_SAMPLETIME_19CYCLES_5
        _19C5,
    #endif

    #ifdef ADC_SAMPLETIME_24CYCLES_5
        _24C5,
    #endif

    #ifdef ADC_SAMPLETIME_39CYCLES_5
        _39C5,
    #endif

    #ifdef ADC_SAMPLETIME_47CYCLES_5
        _47C5,
    #endif

    #ifdef ADC_SAMPLETIME_79CYCLES_5
        _79C5,
    #endif

    #ifdef ADC_SAMPLETIME_92CYCLES_5
        _92C5,
    #endif

    #ifdef ADC_SAMPLETIME_160CYCLES_5
        _160C5,
    #endif

    #ifdef ADC_SAMPLETIME_247CYCLES_5
        _247C5,
    #endif

    #ifdef ADC_SAMPLETIME_640CYCLES_5
        _640C5
    #endif
};
