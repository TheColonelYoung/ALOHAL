/**
 * @file voltage_sensor.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 16.09.2019
 */

#pragma once

#include "sensor.hpp"
#include "adc/adc_channel.hpp"

using namespace std;

class Voltage_sensor: public Sensor
{
private:
    ADC_channel * ADC_ch;

    double resistor_R1 = 0;
    double resistor_R2 = 0;

    double ratio = 1;

public:
    Voltage_sensor(ADC_channel * ADC, double resistor_R1 = 0, double resistor_R2 = 0 );

    double Voltage();
};

