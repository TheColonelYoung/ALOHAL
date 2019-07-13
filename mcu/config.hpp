/**
 * @file config.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 12.07.2019
 */

#pragma once

#define MCU_MODEL "STM32L432"

/* External IRQ support
Enable this to support registring functions to external interupt event handler
*/
// #define EXT_IRQ_EN

// #define ADC_1_EN
// NOTE In higher families MCUs has more ADC, in this purpose conditional compile must be added

// #define DAC_1_EN

//#define UART_1_EN
#define UART_2_EN
// #define UART_3_EN
// #define UART_4_EN


// #define TIM_1_EN
// #define TIM_2_EN
// #define TIM_3_EN
// #define TIM_4_EN
// #define TIM_5_EN
// #define TIM_6_EN
// #define TIM_7_EN
// #define TIM_8_EN
// #define TIM_9_EN
// #define TIM_10_EN
// #define TIM_11_EN
// #define TIM_12_EN
// #define TIM_13_EN
// #define TIM_14_EN
// #define TIM_15_EN

// # define FLASH_EN


// #define OTS_EN


/************************************EXTERNS**********************************************/

#include "global_includes.hpp"

// EXT_IRQ
#ifdef EXT_IRQ_EN
# include "irq/irq_multi_handler.hpp"
extern IRQ_multi_handler<int> EXT_IRQ;
# include "gpio/pin_irq.hpp"
#endif


// FLASH
class Flash_mem;
#ifdef FLASH_EN
# include "flash/flash.hpp"
extern Flash_mem Flash;
#endif


// ADC
class AD_C;
#ifdef ADC_1_EN
# include "adc/ADC.hpp"
extern ADC_HandleTypeDef hadc1;
extern AD_C ADC_1;
#endif

// DAC
class DA_C;
#ifdef DAC_1_EN
# include "dac/DAC.hpp"
extern DAC_HandleTypeDef hdac;
extern DA_C DAC_1;
#endif

// UART
class UART;

#ifdef UART_1_EN
# include "uart/uart.hpp"
extern UART_HandleTypeDef huart1;
extern UART UART_1;
#endif

#ifdef UART_2_EN
# include "uart/uart.hpp"
extern UART_HandleTypeDef huart2;
extern UART UART_2;
#endif

#ifdef UART_3_EN
# include "uart/uart.hpp"
extern UART_HandleTypeDef huart3;
extern UART UART_3;
#endif

#ifdef UART_4_EN
# include "uart/uart.hpp"
extern UART_HandleTypeDef huart4;
extern UART UART_4;
#endif


// TIMERS
class Timer;

#define ALOHAL_CREATE_TIMER(name, handler, size, channels) \
    name = Timer(&handler, size, channels); \
    ALOHAL_TIM_CHAN_BACKPOINTER(name)


#define ALOHAL_TIM_CHAN_BACKPOINTER(timer) \
    for (uint i = 0; i < timer.channel.size(); i++) { \
        timer.channel[i]._parent_timer = &timer; }

#ifdef TIM_1_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim1;
extern Timer TIM_1;
#endif

#ifdef TIM_2_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim2;
extern Timer TIM_2;
#endif

#ifdef TIM_3_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim3;
extern Timer TIM_3;
#endif

#ifdef TIM_4_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim4;
extern Timer TIM_4;
#endif

#ifdef TIM_5_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim5;
extern Timer TIM_5;
#endif

#ifdef TIM_6_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim6;
extern Timer TIM_6;
#endif

#ifdef TIM_7_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim7;
extern Timer TIM_7;
#endif

#ifdef TIM_8_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim8;
extern Timer TIM_8;
#endif

#ifdef TIM_9_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim9;
extern Timer TIM_9;
#endif

#ifdef TIM_10_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim10;
extern Timer TIM_10;
#endif

#ifdef TIM_11_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim11;
extern Timer TIM_11;
#endif

#ifdef TIM_12_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim12;
extern Timer TIM_12;
#endif

#ifdef TIM_13_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim13;
extern Timer TIM_13;
#endif

#ifdef TIM_14_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim14;
extern Timer TIM_14;
#endif

#ifdef TIM_15_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim15;
extern Timer TIM_15;
#endif
