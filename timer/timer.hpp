#ifndef TIMERS_H
#define TIMERS_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_tim.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_tim.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"
#endif

#include "globals.hpp"
#include "irq/irq_handler.hpp"

//---------------USED TIMERS-------------
extern TIM_HandleTypeDef htim2;

//timer 1 input CLK FREW


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
int TIM_set_time(TIM_HandleTypeDef *htim,float useconds);
int TIM_get_prescaler(TIM_HandleTypeDef *htim);
int TIM_get_time(TIM_HandleTypeDef *htim);

class PWM_channel{
    int _index;
    uint32_t _address;
public:
    PWM_channel(int index,uint32_t address);

};

class Timer{
    int size;
    vector<PWM_channel> channel;
    TIM_HandleTypeDef* handler;

public:
    IRQ_handler IRQ;

public:
    Timer() =default;
    Timer(TIM_HandleTypeDef* handler);
    Timer(TIM_HandleTypeDef* handler,int size, int channels);

    int Channel_count(){ return channel.size();}

    void Time_set(float useconds);

    void Counter_set(uint32_t new_counter);
    uint32_t Counter_get();

    void Prescaler_set(uint16_t new_prescaler);
    uint16_t Prescaler_get();

    void Start();
    void Stop();

    void Enable_IRQ();
    void Disable_IRQ();
};


/*              CHANNELS
TIM_CHANNEL_1                      0x00000000U
TIM_CHANNEL_2                      0x00000004U
TIM_CHANNEL_3                      0x00000008U
TIM_CHANNEL_4                      0x0000000CU
TIM_CHANNEL_ALL                    0x00000018U*/

#endif
