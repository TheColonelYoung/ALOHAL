#ifndef UART_H
#define UART_H

#include <string>

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
# include "stm32f0xx_hal_uart.h"
#elif defined STM32_F1
# include "stm32f1xx_hal.h"
# include "stm32f1xx_hal_uart.h"
#elif defined STM32_F3
# include "stm32f3xx_hal.h"
# include "stm32f3xx_hal_uart.h"
#elif defined STM32_F4
# include "stm32f4xx_hal.h"
# include "stm32f4xx_hal_uart.h"
#elif defined STM32_F7
# include "stm32f7xx_hal.h"
# include "stm32f7xx_hal_uart.h"
#endif // ifdef STM32_F0

#include "misc/text_transform.hpp"
#include "gpio/pin.hpp"
#include "main.h"

using namespace std;

// -----Select Enabled UART-----
#define UART1 huart1;
// #define UART2 huart2;
#define UART3 huart3;
// #define UART4 huart4;

class UART;

#ifdef UART1
extern UART_HandleTypeDef huart1;
extern UART UART_1;
#endif

#ifdef UART2
extern UART_HandleTypeDef huart2;
extern UART UART_2;
#endif

#ifdef UART3
extern UART_HandleTypeDef huart3;
extern UART UART_3;
#endif

#ifdef UART4
extern UART_HandleTypeDef huart4;
extern UART UART_4;
#endif



void UART1_IT_Handler();
void UART2_IT_Handler();
void UART3_IT_Handler();
void UART4_IT_Handler();

class UART {
private:
    UART_HandleTypeDef *UART_Handler;

    unsigned char *UART_buffer_temp;
    int buffer_end = 0;

public:
    string UART_buffer = "";

    UART(UART_HandleTypeDef *UART_Handler_set);
    UART(){ };
    int Send(string message); // send strinh over UART
    int Send(int message);    // send number over UART

    int Load(int count); // read specified number of chars from HW buffer to UART_buffer
    int Receive();       // Receive IT Callback -> Insert into

    int Clear_buffer();  // Clear input buffer
};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


#endif // ifndef UART_H
