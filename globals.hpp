#ifndef GLOBALS_H
#define GLOBALS_H

typedef unsigned int uint;

#include <cstdint>

void ALOHAL_init();

// SETTINGS

#define ADC_1_EN
//NOTE In higher families MCUs has more ADC, in this purpose conditional compile must be added

#define UART_1_EN
// #define UART_2_EN
// #define UART_3_EN
// #define UART_4_EN

//ADC
class AD_C;

#ifdef ADC_1_EN
#include "adc/ADC.hpp"
extern ADC_HandleTypeDef hadc;
extern AD_C ADC_1;
#endif

//UART
class UART;

#ifdef UART_1_EN
#include "uart/uart.hpp"
extern UART_HandleTypeDef huart1;
extern UART UART1;
#endif

#ifdef UART_2_EN
#include "uart/uart.hpp"
extern UART_HandleTypeDef huart2;
extern UART UART2;
#endif

#ifdef UART_3_EN
#include "uart/uart.hpp"
extern UART_HandleTypeDef huart3;
extern UART UART3;
#endif

#ifdef UART_4_EN
#include "uart/uart.hpp"
extern UART_HandleTypeDef huart4;
extern UART UART4;
#endif

#endif
