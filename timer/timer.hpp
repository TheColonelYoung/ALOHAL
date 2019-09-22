#ifndef TIMERS_H
#define TIMERS_H

#include "mcu/configuration/settings.hpp"

#if   defined(MCU_FAMILY_STM32_F0)
# include "stm32f0xx_hal.h"
# include "stm32f0xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F1)
# include "stm32f1xx_hal.h"
# include "stm32f1xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F3)
# include "stm32f3xx_hal.h"
# include "stm32f3xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F4)
# include "stm32f4xx_hal.h"
# include "stm32f4xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_F7)
# include "stm32f7xx_hal.h"
# include "stm32f7xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L0)
# include "stm32l0xx_hal.h"
# include "stm32l0xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L1)
# include "stm32l1xx_hal.h"
# include "stm32l1xx_hal_tim.h"
#elif defined(MCU_FAMILY_STM32_L4)
# include "stm32l4xx_hal.h"
# include "stm32l4xx_hal_tim.h"
#endif

#include "globals.hpp"
#include "timer/TIM_channel.hpp"
#include "irq/irq_handler.hpp"

#define TIM_FREQ (80000000)

// ---------------USED TIMERS-------------
extern TIM_HandleTypeDef htim2;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
int TIM_set_time(TIM_HandleTypeDef *htim, float useconds);
int TIM_get_prescaler(TIM_HandleTypeDef *htim);
int TIM_get_time(TIM_HandleTypeDef *htim);

//TODO calculation of max time,

class Timer {
    int size;
    bool optimize = false;
    float uticks;
    uint frequency;


    TIM_HandleTypeDef *handler;

public:
    IRQ_handler IRQ;
    vector<TIM_channel> channel;

public:
    Timer() = default;
    Timer(TIM_HandleTypeDef *handler);
    Timer(TIM_HandleTypeDef *handler, int size, int channels);

    int Channel_count(){ return channel.size(); }

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

#endif // ifndef TIMERS_H
