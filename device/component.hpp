/**
 * @file component.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 30.08.2019
 */

#pragma once

#include <string>

typedef unsigned int uint;

class Device;
extern Device *device;

using namespace std;

/**
 * @brief   Represents parts on PCB which is connected with mcu someway
 *          Used for any parts of device, for example some I2C memory or stepper motor drivers
 */
class Component
{
private:
    string name = "Unknown_component";

public:
    /**
     * @brief Construct a new Component object, with enumerated name and register it to device
     *
     * @param name Original name for Component, example L6470
     */
    Component(string name);

    /**
     * @brief Variable Name gettor
     *
     * @return string Name of component
     */
    string Name() const;
};
