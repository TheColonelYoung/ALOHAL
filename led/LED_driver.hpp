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
    /**
     * @brief   Construct a new led driver object
     *          Uses default name for component
     *          Should not be used in production
     *
     * @param maximal_current   Output current at 100% power
     * @param allowed_current   Maximal allowed current on output
     */
    LED_Driver(uint maximal_current, uint allowed_current);

    /**
     * @brief   Construct a new led driver object
     *          Used by derivated LED drivers
     *
     * @param name              Name of component
     * @param maximal_current   Output current at 100% power
     * @param allowed_current   Maximal allowed current on output
     */
    LED_Driver(string name, uint maximal_current, uint allowed_current);

    /**
     * @brief Destroy the led driver object
     */
    ~LED_Driver() = default;

    /**
     * @brief       Set power to percentage of maximal power (percentage of maximal current)
     *
     * @param power Percentage of maximal power
     */
    virtual void Power(float power) = 0;

    /**
     * @brief           Return how much power was setup
     *
     * @return float    Actual power level
     */
    virtual float Power() = 0;

    /**
     * @brief           Set current in uA
     *
     * @param current   Output current in uA
     */
    virtual void Current(uint current) = 0;

    /**
     * @brief       Read the set current
     *
     * @return uint Actual current in uA
     */
    virtual uint Current() = 0;
};
