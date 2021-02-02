/**
 * @file TIM_channel.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.2
 * @date 01.04.2020
 */

#pragma once

#include "mcu/configuration/settings.hpp"
#include "global_includes.hpp"

#include <vector>
#include <string>

class Timer;

using namespace std;

class TIM_channel {

public:
    enum class Modes {
        PWM,
        Toggle
    };

private:
    int _index;
    uint32_t _address;

    Modes mode = Modes::PWM;


public:
    Timer *_parent_timer;

    TIM_channel(Timer *parent, uint32_t address);

    uint32_t  Address() const {return _address;};

    /**
     * @brief           Set mode of timer
     *
     * @param mode      New mode of timer
     */
    void Mode(Modes mode);

    /**
     * @brief           Return actual mode of timer
     *
     * @return Modes    Actual mode of timer
     */
    inline Modes Mode(){ return mode; };

    int PWM_Generate(float percentage);

    void Start();

    void Stop();

    void Start_DMA(uint32_t *address_from, uint32_t data_size);

    void Pulse(uint32_t pulse_length);
};
