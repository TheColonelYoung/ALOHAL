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

/**
 * @brief   Basic voltage measuring object. Assumes that voltage is directly connected
 *              to input ADC pin. So that no voltage divider is used.
 */
class Voltage_sensor : public Sensor
{
protected:

    /**
     * @brief   ADC input channel for voltage readings
     */
    ADC_channel *ADC_ch;

public:

    /**
     * @brief   Construct a new Voltage_sensor object
     *          Used by derivated sensors
     *
     * @param name      Name of sensor, used by derivated sensors otherwise is default
     *                      pass name to Sensor constructor
     * @param ADC_IN    ADC input channel for voltage readings
     */
    Voltage_sensor(ADC_channel *ADC_IN, string name = "Voltage_sensor");

    /**
     * @brief   Read voltage from input channel. Voltage is referenced to Supply_voltage()
     *
     * @return double Voltage on ADC input
     */
    virtual double Voltage();
};
