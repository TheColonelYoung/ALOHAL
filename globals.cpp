#include "globals.hpp"

void ALOHAL_init(){
    // ADC
    #ifdef ADC_1_EN
    ADC_1 = AD_C(&hadc1);
    #endif

    // FLASH
    #ifdef FLASH_EN
    Flash = Flash_mem();
    #endif

    // UART
    #ifdef UART_1_EN
    UART_1 = UART(&huart1);
    #endif

    #ifdef UART_2_EN
    UART_2 = UART(&huart2);
    #endif

    #ifdef UART_3_EN
    UART_3 = UART(&huart3);
    #endif
    #ifdef UART_4_EN
    UART_4 = UART(&huart4);
    #endif

    // DAC
    #ifdef DAC_1_EN
    DAC_1 = DA_C(&hdac, 2);
    #endif

    #ifdef TIM_1_EN
    ALOHAL_CREATE_TIMER(TIM_1, htim1, 16, 4)
    #endif
    #ifdef TIM_2_EN
    ALOHAL_CREATE_TIMER(TIM_2, htim2, 32, 4)
    #endif
    #ifdef TIM_3_EN
    ALOHAL_CREATE_TIMER(TIM_3, htim3, 16, 4)
    #endif
    #ifdef TIM_4_EN
    ALOHAL_CREATE_TIMER(TIM_4, htim4, 16, 4)
    #endif
    #ifdef TIM_5_EN
    ALOHAL_CREATE_TIMER(TIM_5, htim5, 32, 4)
    #endif
    #ifdef TIM_6_EN
    ALOHAL_CREATE_TIMER(TIM_6, htim6, 16, 0)
    #endif
    #ifdef TIM_7_EN
    ALOHAL_CREATE_TIMER(TIM_7, htim7, 16, 0)
    #endif
    #ifdef TIM_8_EN
    ALOHAL_CREATE_TIMER(TIM_8, htim8, 16, 4)
    #endif
    #ifdef TIM_9_EN
    ALOHAL_CREATE_TIMER(TIM_9, htim9, 16, 2)
    #endif
    #ifdef TIM_10_EN
    ALOHAL_CREATE_TIMER(TIM_10, htim10, 16, 1)
    #endif
    #ifdef TIM_11_EN
    ALOHAL_CREATE_TIMER(TIM_11, htim11, 16, 1)
    #endif
    #ifdef TIM_12_EN
    ALOHAL_CREATE_TIMER(TIM_12, htim12, 16, 2)
    #endif
    #ifdef TIM_13_EN
    ALOHAL_CREATE_TIMER(TIM_13, htim13, 16, 1)
    #endif
    #ifdef TIM_14_EN
    ALOHAL_CREATE_TIMER(TIM_14, htim14, 16, 1)
    #endif
    #ifdef TIM_15_EN
    ALOHAL_CREATE_TIMER(TIM_15, htim15, 16, 4)
    #endif

    #ifdef OTS_EN
    OTS = Tasker();
    #endif

} // ALOHAL_init

// EXT_IRQ
#ifdef EXT_IRQ_EN
IRQ_multi_handler<int> EXT_IRQ = IRQ_multi_handler<int>();
#endif

// FLASH
#ifdef FLASH_EN
Flash_mem Flash;
#endif

// ADC
#ifdef ADC_1_EN
AD_C ADC_1;
#endif

// DAC
#ifdef DAC_1_EN
DA_C DAC_1;
#endif

// UART
#ifdef UART_1_EN
UART UART_1;
#endif
#ifdef UART_2_EN
UART UART_2;
#endif
#ifdef UART_3_EN
UART UART_3;
#endif
#ifdef UART_4_EN
UART UART_4;
#endif

#ifdef TIM_1_EN
Timer TIM_1;
#endif
#ifdef TIM_2_EN
Timer TIM_2;
#endif
#ifdef TIM_3_EN
Timer TIM_3;
#endif
#ifdef TIM_4_EN
Timer TIM_4;
#endif
#ifdef TIM_5_EN
Timer TIM_5;
#endif
#ifdef TIM_6_EN
Timer TIM_6;
#endif
#ifdef TIM_7_EN
Timer TIM_7;
#endif
#ifdef TIM_8_EN
Timer TIM_8;
#endif
#ifdef TIM_9_EN
Timer TIM_9;
#endif
#ifdef TIM_10_EN
Timer TIM_10;
#endif
#ifdef TIM_11_EN
Timer TIM_11;
#endif
#ifdef TIM_12_EN
Timer TIM_12;
#endif
#ifdef TIM_13_EN
Timer TIM_13;
#endif
#ifdef TIM_14_EN
Timer TIM_14;
#endif
#ifdef TIM_15_EN
Timer TIM_15;
#endif

#ifdef OTS_EN
Tasker OTS;
#endif
