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
 * @brief   Abstract class for LED current driver
 *          All values are in uA, LED driver is probably not capable of this resolution
 *          Works with current ratings and setting of maximal allowed current
 *          Current is set in procentage of maximal current or by value
 */
class LED_Driver : public Component {
    
protected:
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
     * @param maximal_current   Output current at 100% power in uA
     * @param allowed_current   Maximal allowed current on output in uA
     */
    LED_Driver(uint maximal_current, uint allowed_current = 0);

    /**
     * @brief   Construct a new led driver object
     *          Used by derivated LED drivers
     *
     * @param name              Name of component
     * @param maximal_current   Output current at 100% power in uA
     * @param allowed_current   Maximal allowed current on output in uA
     */
    LED_Driver(string name, uint maximal_current, uint allowed_current = 0);

    /**
     * @brief Destroy the led driver object
     */
    ~LED_Driver() = default;

    /**
     * @brief       Set power to percentage of maximal power (percentage of maximal current)
     *              Assuming that power scaling is linear to current scaling
     *              Example: 100 is 100 mA => 50% is 50 mA
     *
     * @param power Percentage of maximal power
     */
    virtual double Power(float power);

    /**
     * @brief           Return how much power was setup
     *
     * @return float    Actual power level
     */
    virtual double Power() const;

    /**
     * @brief       Return value of power limit in percentage of maximal power
     *              Restricted by allowed_current
     *
     * @return float    Percentage of maximal current output, which is allowed
     */
    virtual double Power_limit() const;

    /**
     * @brief           Set current in uA
     *
     * @param current   Output current in uA
     */
    virtual uint Current(uint current_ua) = 0;

    /**
     * @brief       Read the set current
     *
     * @return uint Actual current in uA
     */
    virtual uint Current() const {return actual_current;};

    /**
     * @brief       Read the set current
     *
     * @return uint Maximal allowed current in uA
     */
    virtual uint Current_limit() const {return allowed_current;};
};
