/**
 * @file potentiometer.hpp
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
 * @brief   Measure and then calculate voltage on potenciometer.
 *          Can also calculate percentage on which is potenciometer set.
 *          Also support trimming af voltage to calibrate endpoint of potentiometer.
 *          Resistor R1 is used to limit current when potentiometer is set to 0.
 *          Input voltage should not be higher then ADC reference voltage.
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
    |-| R_VAR_1
    |-|
    +++
     |
    +-+ GND
     +
 */
class Potentiometer : public Voltage_sensor
{
private:

    /**
     * @brief   All reading which voltage is below is rounded to zero
     */
    double trim_upper = -1;

    /**
     * @brief   All reading which voltage is below is rounded to maximal value
     */
    double trim_lower = -1;

    /**
     * @brief   Maximal voltage that can apper on ADC input based on resistor, potentiometer and input voltage values.
     *          Minimal value is assumed to be zero.
     */
    double maximal_voltage = -1;

public:

    /**
     * @brief Construct a new Potentiometer object
     *
     * @param ADC_IN                    ADC input channel for voltage readings
     * @param resistor_R1               Value of upper resistor in divider in ohms
     * @param potentiometer_R_VAR_1     Maximal value of potentiometer in divider in ohms, minimal resistence
     *                                      is assumed to be 0 ohm
     */
    Potentiometer(ADC_channel *ADC_IN, double resistor_R1 = 10000, double potentiometer_R_VAR_1 = 10000, double input_voltage = 3.3);

    /**
     * @brief   Returns percentage which is set by potenciometer.
     *          Pot at maximal value is 100%, at minimal is 0%, can be trimmed by trim values.
     *
     * @return double   Percentage set by potenciometer.
     */
    double Percentage();

    /**
     * @brief   Set minimal and maximal trim values. All below minimal_voltage (lower_trim) is 0%. All above
     *              maximal_voltage (upper_trim) is 100%. Values between are distributed lineary.
     *          Can be used to correct imperfect values of resistor, when for example real maximal value is only 99%.
     *
     * @param minimal_voltage   trim_lower, all below is 0%
     * @param maximal_voltage   trim_upper, all above is 100%
     *
     * @return true     Trim points are valid
     * @return false    Trim points are invalid (maximal in lower then minimal)
     */
    bool Trim(double minimal_voltage, double maximal_voltage);
};
