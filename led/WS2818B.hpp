/**
 * @file WS2818B.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief
 * @version 0.1
 * @date 01.04.2020
 */

#pragma once

#include <vector>
#include <string>

#include "globals.hpp"
#include "global_includes.hpp"

#include "device/component.hpp"
#include "timer/timer.hpp"
#include "timer/TIM_channel.hpp"

typedef unsigned int uint;

using namespace std;

/**
 * @brief   Controller of RGB LED. Comunicate via fat 1MHz procol with PWM.
 *          Can be chained but this function is now not supported.
 *          Support 255 intensity level of all colors, allow mixing of colors.
 *          For operation request one Timer and channel which is capable of output compare mode.
 *          For fast protocol generating, DMA is used with combination of Output compare.
 *
 *          Pin must be configured for fast operation and with pull down resistor
 *          DMA must be configured for transfer from memory to peripheral, with address increment.
 *          DMA must be in Normal mode, memory size 32bits, Peripheral 16-bit or 32-bit based on timer.
 */
class WS2818B : public Component
{
private:
    /**
     * @brief Length of logic levels of protocol in nanosecond
     */
    const int logic_LOW    = 300;
    const int logic_HIGH   = 700;

    /**
     * @brief Length of logic levels in number of timer ticks
     */
    uint tick_LOW = 0;
    uint tick_HIGH = 0;

    /**
     * @brief Maximal valueof timer counter
     */
    uint timer_max_count = 0;

    /**
     * @brief Pointer to timer on mcu which will be controling signal pin
     */
    Timer *timer;

    /**
     * @brief Index of Timer channel, which will be used for output compare method of protocol generating
     */
    uint8_t channel_index;

    /**
     * @brief Number of chained LEDs, now not used
     */
    uint16_t chain_count;

    /**
     * @brief Actual color of LED
     */
    vector<uint8_t> color = { 0x00, 0x00, 0x00 };

public:
    WS2818B(Timer *timer, uint8_t channel_index, uint16_t chain_count = 1);
    ~WS2818B() = default;

    double LED_count(){ return chain_count; };

    int Color(vector<uint8_t> color);
    int Color(uint8_t red, uint8_t green, uint8_t blue);

    void Timer_stop();

private:

    int Init_timer();
    vector<uint32_t> Protocol_generator(uint8_t red, uint8_t green, uint8_t blue);
    void Push_protocol(vector<uint32_t> &protocol_timing);

    inline void Generate_HIGH(vector<uint32_t> &frame);
    inline void Generate_LOW(vector<uint32_t> &frame);
};
