/**
 * @file timer.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.2
 * @date 01.04.2020
 */
#pragma once

#include "global_includes.hpp"
#include "globals.hpp"
#include "timer/TIM_channel.hpp"
#include "irq/irq_handler.hpp"

class Timer {
    /**
     * @brief Size of timer counter in bits
     */
    uint8_t size;

    bool optimize = false;
    float uticks;
    uint frequency;

    TIM_HandleTypeDef *handler;

public:
    IRQ_handler *IRQ = new IRQ_handler();
    vector<TIM_channel> channel;

public:
    Timer() = default;
    Timer(TIM_HandleTypeDef *handler);
    Timer(TIM_HandleTypeDef *handler, int size, int channels);

    int Channel_count(){ return channel.size(); }
    uint8_t Size(){return size;};


    void Time_set(float useconds);
    void Frequency_set(float frequency);
    bool Optimize(bool flag); // set flag for optimizing of prescaler for actual time

    void Counter_set(uint32_t new_counter);
    uint32_t Counter_get();

    void Prescaler_set(uint16_t new_prescaler);
    uint16_t Prescaler();

    uint Input_frequency();         //frequency of source for counter
    TIM_HandleTypeDef * Handler();  //return own handler

    void Start();
    void Stop();

    void Enable_IRQ();
    void Disable_IRQ();

private:
    void Optimize_for(int time_us); // opzimize prescaler for given time
};

/*              CHANNELS
 * TIM_CHANNEL_1                      0x00000000U
 * TIM_CHANNEL_2                      0x00000004U
 * TIM_CHANNEL_3                      0x00000008U
 * TIM_CHANNEL_4                      0x0000000CU
 * TIM_CHANNEL_ALL                    0x00000018U*/
