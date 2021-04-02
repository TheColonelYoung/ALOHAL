/**
 * @file NCL30160.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 02.04.2020
 */

#pragma once

#include "LED_driver.hpp"
#include "timer/TIM_channel.hpp"

using namespace std;

class NCL30160: public LED_Driver
{
private:
    /**
     * @brief Timer channel which is generating PWM for NCL30160 DIM input
     */
    TIM_channel *source_channel = nullptr;

public:

    NCL30160(TIM_channel *source_channel, uint maximal_current, uint allowed_current = 0);

    ~NCL30160() = default;

    /**
     * @brief           Set current in uA
     *
     * @param current   Output current in uA
     */
    virtual uint Set_output_current(uint current) final override;
};
