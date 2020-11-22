/**
 * @file adc_channel.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 14.09.2019
 */

#pragma once

#include "adc.hpp"

class ADC_channel
{
private:
    AD_C *ADC_IN;

    int channel;

public:
    ADC_channel() =default;
    ADC_channel(AD_C *ADC_IN, int channel);

    /**
     * @brief Perform measurement on given channel
     *
     * @return double
     */
    double Measure_poll();

};
