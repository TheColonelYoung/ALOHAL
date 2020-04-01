/**
 * @file LED_Driver.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.2
 * @date 01.04.2020
 */

#pragma once

#include "device/component.hpp"

using namespace std;

/**
 * @brief   Interface for LED current driver
 *          All values are in uA
 *          Works with current ratings and setting of maximal allowed current
 *          Current is set in procentage of maximal current or by value
 */
class LED_Driver : public Component {
private:

    /**
     * @brief Current at 100% power
     */
    uint maximal_current;

    /**
     * @brief   Maximal allowed current
     *          Set to 0 for no limitation
     */
    uint allowed_current;

    /**
     * @brief Actual value of current which was set
     */
    uint actual_current = 0;

public:
    LED_Driver(uint maximal_current, uint allowed_current);
    LED_Driver(string name, uint maximal_current, uint allowed_current);

    ~LED_Driver() = default;

    virtual void Power(float power) = 0;
    virtual float Power() = 0;

    virtual void Current(uint current) = 0;
    virtual uint Current() = 0;
};
