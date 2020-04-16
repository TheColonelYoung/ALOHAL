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
    int _index;
    uint32_t _address;

public:
    Timer *_parent_timer;

    TIM_channel(Timer *parent, uint32_t address);

    uint32_t  Address() const {return _address;};

    int Set_mode(string mode);

    int PWM_Generate(float percentage);
    void PWM_Start();
    void PWM_Stop();

};
