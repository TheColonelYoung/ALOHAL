#ifndef TIMERS_H
#define TIMERS_H

/*
Instert to main.c before Main

--> USER CODE BEGIN 0
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Timer_IT_PeriodElapsedCallback(htim);
}
--> USER CODE END 0

 AND

--> USER CODE BEGIN 2

HAL_TIM_Base_Start_IT(&htimX);

-->USER CODE END 2

 */

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

#include "gpio/pin.hpp"
#include "globals.hpp"
#include "timer/PWM.hpp"

//---------------USED TIMERS-------------
extern TIM_HandleTypeDef htim2;

//timer 1 input CLK FREW
#define TIM_FREQ 72000000

void Timer_IT_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
int TIM_set_time(TIM_HandleTypeDef *htim,float useconds);
int TIM_get_prescaler(TIM_HandleTypeDef *htim);
int TIM_get_time(TIM_HandleTypeDef *htim);

class Timer{
    int size;
    vector<PWM_channel> channels;
    TIM_HandleTypeDef* handler;

public:
    Timer() =default;
    Timer(TIM_HandleTypeDef* handler);
    Timer(TIM_HandleTypeDef* handler,int size, int channels);

    void IRQ_Overflow();

    inline int Channel_count(){ return channels.size();}

    void Time_set(float useconds);

    inline void Counter_set(uint32_t new_counter);
    inline uint32_t Counter_get();

    inline void Prescaler_set(uint16_t new_prescaler);
    inline uint16_t Prescaler_get();

};

class PWM_channel{
    int _index
public:
    PWM_channel();

};


#endif
