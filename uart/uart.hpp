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

using namespace std;

void UART1_IT_Handler();
void UART2_IT_Handler();
void UART3_IT_Handler();
void UART4_IT_Handler();

// class IRQ_handler;

class UART {
private:
    UART_HandleTypeDef *UART_Handler;

    unsigned char *UART_buffer_temp;

    vector<string> TX_buffer;

    int buffer_end = 0;

public:
    IRQ_handler IRQ;

    string RX_buffer = "";
    bool busy = false;

    UART(UART_HandleTypeDef *UART_Handler_set);
    UART(){ };


    int Send(string message); // send string over UART

    template <typename send__type> // send any type as string via UART
    int Send(send__type message){
        return Send(to_string(message));
    }

    int Resend(); // routine that send another string from buffer if UART is avaible

    int Load();    // Copy data from temp buffer to class buffer
    int Receive(); // Receive IT Callback -> Insert into

    int Clear_buffer(); // Clear input buffer
};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart); // Callback called after any receive
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart); // Callback called after any send


#endif // ifndef UART_H
