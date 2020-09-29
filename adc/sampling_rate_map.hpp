/**
 * @file sampling_rate_map.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief
 * @version 0.1
 * @date 17.08.2020
 */

#pragma once

#include <map>
#include "sampling_rate_values.hpp"

std::map<Sampling_rate, uint32_t> sample_rate_map{
    #ifdef ADC_SAMPLETIME_1CYCLE_5
        make_pair(Sampling_rate::_1C5, ADC_SAMPLETIME_1CYCLE_5),
    #endif

    #ifdef ADC_SAMPLETIME_2CYCLES_5
        make_pair(Sampling_rate::_2C5, ADC_SAMPLETIME_2CYCLE_5),
    #endif

    #ifdef ADC_SAMPLETIME_3CYCLES_5
        make_pair(Sampling_rate::_3C5, ADC_SAMPLETIME_3CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_6CYCLES_5
        make_pair(Sampling_rate::_6C5, ADC_SAMPLETIME_6CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_7CYCLES_5
        make_pair(Sampling_rate::_7C5, ADC_SAMPLETIME_7CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_12CYCLES_5
        make_pair(Sampling_rate::_12C5, ADC_SAMPLETIME_12CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_19CYCLES_5
        make_pair(Sampling_rate::_19C5, ADC_SAMPLETIME_19CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_24CYCLES_5
        make_pair(Sampling_rate::_24C5, ADC_SAMPLETIME_24CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_39CYCLES_5
        make_pair(Sampling_rate::_39C5, ADC_SAMPLETIME_39CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_47CYCLES_5
        make_pair(Sampling_rate::_47C5, ADC_SAMPLETIME_47CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_79CYCLES_5
        make_pair(Sampling_rate::_79C5, ADC_SAMPLETIME_79CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_92CYCLES_5
        make_pair(Sampling_rate::_92C5, ADC_SAMPLETIME_92CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_160CYCLES_5
        make_pair(Sampling_rate::_160C5, ADC_SAMPLETIME_160CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_247CYCLES_5
        make_pair(Sampling_rate::_247C5, ADC_SAMPLETIME_247CYCLES_5),
    #endif

    #ifdef ADC_SAMPLETIME_640CYCLES_5
        make_pair(Sampling_rate::_640C5, ADC_SAMPLETIME_640CYCLES_5)
    #endif
};
