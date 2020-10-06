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
#include <algorithm>

#include "globals.hpp"
#include "global_includes.hpp"

#include "device/component.hpp"
#include "timer/timer.hpp"
#include "timer/TIM_channel.hpp"
#include "misc/color.hpp"

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

public:
    /**
     * @brief Construct a new WS2818B object
     *
     * @param timer             Timer which is driving the DMA
     * @param channel_index     Channel of timer which is in mode Output compare
     * @param chain_count       Number of LEDs in chain now only one is supported
     */
    WS2818B(Timer *timer, uint8_t channel_index, uint16_t chain_count = 1);
    ~WS2818B() = default;

    /**
     * @brief   Returns number of LED in chain
     *
     * @return double Number of LED in chain
     */
    double LED_count(){ return chain_count; };

    /**
     * @brief Set color of first LED in chain
     *
     * @param color         New color of first LED in chain
     * @param intensity     Intensity of color from range 0.0 to 1.0
     * @return int          Length of protocol sequence
     */
    int Color(Color::Colors color, float intensity = 1.0);

    /**
     * @brief   Set color of first LED in chain
     *
     * @param red       Intensity of red 0-255
     * @param green     Intensity of green 0-255
     * @param blue      Intensity of blue 0-255
     * @param intensity Global intensity modifier
     * @return int      Length of protocol sequence
     */
    int Color(uint8_t red, uint8_t green, uint8_t blue, float intensity = 1.0);

    /**
     * @brief   Callback to stop timer after whole sequence is transmitted
     *          This callback is registred to Timer IRQ
     */
    void Timer_stop();

private:
    /**
     * @brief   Initialize timer to OC mode and setup initial values (prescaler, period, ARR preload)
     *
     * @return int  0 - all OK
     *              1 - Timer config failed
     *              2 - Channel init failed
     *              3 - Channel config failed
     */
    int Init_timer();

    /**
     * @brief   Generate sequence of incremental delays in protocol, in which will be output toggled
     *
     * @param red       Intensity of red 0-255
     * @param green     Intensity of green 0-255
     * @param blue      Intensity of blue 0-255
     * @return vector<uint32_t>     Vector of delay
     */
    vector<uint32_t> Protocol_generator(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Initiate DMA transfer from memory to Timer register
     *
     * @param protocol_timing Vector with delays of protocol
     */
    void Push_protocol(vector<uint32_t> &protocol_timing);

    /**
     * @brief Add HIGH value into protocol frame
     *
     * @param frame Vector with delays of protocol
     */
    inline void Generate_HIGH(vector<uint32_t> &frame);

    /**
     * @brief Add LOW value into protocol frame
     *
     * @param frame Vector with delays of protocol
     */
    inline void Generate_LOW(vector<uint32_t> &frame);
};
