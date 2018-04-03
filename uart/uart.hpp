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

#include <vector>
#include "globals.hpp"
#include "misc/text_transform.hpp"
#include "irq/irq_handler.hpp"
#include "gpio/pin.hpp"

using namespace std;

void UART1_IT_Handler();
void UART2_IT_Handler();
void UART3_IT_Handler();
void UART4_IT_Handler();

//class IRQ_handler;

class UART {
private:
    UART_HandleTypeDef *UART_Handler;

    unsigned char *UART_buffer_temp;

    unsigned char *RX_buffer;
    vector<string> TX_buffer;

    int buffer_end = 0;

public:
    IRQ_handler IRQ;

    string UART_buffer = "";
    bool busy = false;

    UART(UART_HandleTypeDef *UART_Handler_set);
    UART(){ };

    int Send(string message); // send string over UART
    int Send(int message);    // send number over UART
    int Send(uint message);    // send number over UART

    int Resend();

    int Load();
    int Receive();       // Receive IT Callback -> Insert into

    int Clear_buffer();  // Clear input buffer
};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);


#endif // ifndef UART_H
