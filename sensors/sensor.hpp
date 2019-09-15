/**
 * @file sensor.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 15.09.2019
 */

#pragma once

#include "device/component.hpp"

using namespace std;

class Sensor: public Component
{
private:
    /* data */
public:
    Sensor();
    Sensor(string name);

    /**
     * @brief
     *
     * @return vector<string> Value measurable by sensor
     */
    vector<string> Measurable_values();

    /**
     * @brief Perform measuring of selected value
     *
     * @param value
     * @return double
     */
    double Measure(string value);
};




