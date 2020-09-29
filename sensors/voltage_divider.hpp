/**
 * @file voltage_divider.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 21.08.2020
 */

#pragma once

#include "sensor.hpp"
#include "adc/adc_channel.hpp"
#include "sensors/voltage_sensor.hpp"

using namespace std;

/**
 * @brief   Measure and then calculate voltage above the voltage divider made by two resistors of known value.
 *
   Schema:

     +  Input Voltage
     |
    +++
    | | R1
    | |
    +++
     |
     +-----+  ACD Input
     |
    +++
    | | R2
    | |
    +++
     |
    +-+ GND
     +
 */
class Voltage_divider : public Voltage_sensor
{
private:

    /**
     * @brief Ratio of voltage divider
     */
    double ratio = 1;

public:

    /**
     * @brief Construct a new Voltage_divider object
     *
     * @param ADC_IN        ADC input channel for voltage readings
     * @param resistor_R1   Value of upper resistor in divider in ohms
     * @param resistor_R2   Value of lower resistor in divider in ohms
     */
    Voltage_divider(ADC_channel *ADC_IN, double resistor_R1 = 10000, double resistor_R2 = 10000);

    /**
     * @brief   Read and then calculated voltage connected to voltage divider
     *
     * @return double   Voltage on input of divider
     */
    virtual double Voltage() override;
};
