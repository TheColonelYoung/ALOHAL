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

TIM_TypeDef *TIM0;
TIM_TypeDef *TIM1;
TIM_TypeDef *TIM2;
TIM_TypeDef *TIM3;
TIM_TypeDef *TIM4;
TIM_TypeDef *TIM5;
TIM_TypeDef *TIM7;
TIM_TypeDef *TIM8;

#include "gpio/pin.hpp"
#include "timer/tasker.hpp"
#include "motor/SM_DRV8825.hpp"
#include "timer/PWM.hpp"

//---------------USED TIMERS-------------
extern TIM_HandleTypeDef htim2;

//timer 1 input CLK FREW
#define TIM_FREQ 72000000

void Timer_IT_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
int TIM_set_time(TIM_HandleTypeDef *htim,float useconds);
int TIM_get_prescaler(TIM_HandleTypeDef *htim);
int TIM_get_time(TIM_HandleTypeDef *htim);
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

#endif
