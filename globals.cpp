#include "globals.hpp"

void ALOHAL_init(){

    //ADC
    #ifdef ADC_1_EN
    ADC_1 = AD_C(&hadc);
    #endif

    //UART
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

}

//ADC
#ifdef ADC_1_EN
AD_C ADC_1;
#endif

//UART
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
