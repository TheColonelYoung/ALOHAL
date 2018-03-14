#include "globals.hpp"

void ALOHAL_init(){
    // ADC
    #ifdef ADC_1_EN
    ADC_1 = AD_C(&hadc);
    #endif

    //FLASH
    #ifdef FLASH_EN
    Flash = Flash_mem();
    #endif

    // UART
    #ifdef UART_1_EN
    UART1 = UART(&huart1);
    #endif
    #ifdef UART_2_EN
    UART2 = UART(&huart2);
    #endif
    #ifdef UART_3_EN
    UART3 = UART(&huart3);
    #endif
    #ifdef UART_4_EN
    UART4 = UART(&huart4);
    #endif

    #ifdef TIM_1_EN
    TIM_1 = Timer(&htim1,16,4);
    #endif
    #ifdef TIM_2_EN
    TIM_2 = Timer(&htim2);
    #endif
    #ifdef TIM_3_EN
    TIM_3 = Timer(&htim3);
    #endif
    #ifdef TIM_4_EN
    TIM_4 = Timer(&htim4);
    #endif
    #ifdef TIM_5_EN
    TIM_5 = Timer(&htim5);
    #endif
    #ifdef TIM_6_EN
    TIM_6 = Timer(&htim6);
    #endif
    #ifdef TIM_7_EN
    TIM_7 = Timer(&htim7);
    #endif
    #ifdef TIM_8_EN
    TIM_8 = Timer(&htim8);
    #endif
} // ALOHAL_init


//FLASH
#ifdef FLASH_EN
Flash_mem Flash;
#endif

// ADC
#ifdef ADC_1_EN
AD_C ADC_1;
#endif

// UART
#ifdef UART_1_EN
UART UART1;
#endif
#ifdef UART_2_EN
UART UART2;
#endif
#ifdef UART_3_EN
UART UART3;
#endif
#ifdef UART_4_EN
UART UART4;
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
