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

/**
 * @brief   Channel of timer, contains capture-compare register, which is compared to ARR of timer.
 *
 *          Channel must be configured with:
 *          Polarity: high, Fast: disable
 */
class TIM_channel{

public:
    /**
     * @brief   Available modes of channel
     *
     *          PWM - When ARR and CCR are equal, falling edge is generated, rising edge is on 0 of ARR
     *          Toggle - When ARR and CCR are equal outputs toggle
     */
    enum class Modes {
        PWM,
        Toggle
    };

private:
    /**
     * @brief Address of timer channel in memory
     */
    uint32_t _address;

    /**
     * @brief Current mode of timers channel
     */
    Modes mode = Modes::PWM;

public:
    /**
     * @brief Pointer to parent timer
     */
    Timer *parent_timer;

    /**
     * @brief Construct a new tim channel object
     *
     * @param parent    Pointer to parent timer
     * @param address   Address of timer channel, 0 fot CH1, 0x4 for TIM2, ...
     */
    TIM_channel(Timer *parent, uint32_t address);

    /**
     * @brief Return address of timer channel in memory
     *
     * @return uint32_t Address of timer channel in memory
     */
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

    /**
     * @brief If Channel is configured as PWM, this will calculate value for Capture compare register for given duty cycle
     *
     * @param duty_cycle Duty cycle of PWM signal
     * @return int
     */
    int PWM_Generate(float duty_cycle);

    /**
     * @brief Returns index of channel, For channel 1 is 1
     *
     * @return unsigned short Index of channel
     */
    unsigned short Index() {return (_address/4)+1;};

    /**
     * @brief Starts timer channel
     */
    void Start();

    /**
     * @brief Starts timer channel
     */
    void Stop();

    /**
     * @brief Starts DMA transfer, transfer must be preconfigured in CubeMX
     *
     * @param address_from  Source address
     * @param data_size     Number of data packages in memory, transfered size must be preconfigured
     */
    void Start_DMA(uint32_t *address_from, uint32_t data_size);

private:

    /**
     * @brief Reads capture-compare register of timer
     *
     * @return uint32_t Value of capture-compare register
     */
    inline uint32_t Capture_compare();

    /**
     * @brief Set value to capture-compare register
     *
     * @param capture_compare   New value of capture-compare register
     */
    inline void Capture_compare(uint32_t capture_compare);
};
