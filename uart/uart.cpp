#include "uart.hpp"

unsigned char UART_1_buffer_temp[10];
unsigned char UART_2_buffer_temp[10];
unsigned char UART_3_buffer_temp[10];
unsigned char UART_4_buffer_temp[10];

UART::UART(UART_HandleTypeDef *UART_Handler_set){
    UART_Handler = UART_Handler_set;

    #ifdef UART1
    if (UART_Handler->Instance == USART1) {
        UART_buffer_temp = UART_1_buffer_temp;
    }
    #endif

    #ifdef UART2
    if (UART_Handler->Instance == USART2) {
        UART_buffer_temp = UART_2_buffer_temp;
    }
    #endif

    #ifdef UART3
    if (UART_Handler->Instance == USART3) {
        UART_buffer_temp = UART_3_buffer_temp;
    }
    #endif

    #ifdef UART4
    if (UART_Handler->Instance == USART4) {
        UART_buffer_temp = UART_4_buffer_temp;
    }
    #endif

    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
}

int UART::Send(string message){
    HAL_UART_Transmit(UART_Handler, (unsigned char *) message.c_str(), message.length(), message.length());
    return message.length();
}

int UART::Send(int number){
    string message = itoa(number);
    HAL_UART_Transmit(UART_Handler, (unsigned char *) message.c_str(), message.length(), message.length());
    return message.length();
}

int UART::Load(int count){
    UART_buffer.push_back(UART_buffer_temp[0]);
    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
    return count;
}

int UART::Clear_buffer(){
    UART_buffer = "";
    return UART_buffer.length();
}

void UART1_IT_Handler(){
    UART_1.Send(UART_1.UART_buffer);
    UART_1.Clear_buffer();
}

void UART2_IT_Handler(){ }

void UART3_IT_Handler(){
    //UART_1.Send(UART_3.UART_buffer.length());
    ESP_Receive();
}

void UART4_IT_Handler(){ }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    #ifdef UART1
    if (huart->Instance == USART1) {
        UART_1.Load(1);
        UART1_IT_Handler();
    }
    #endif
    #ifdef UART2
    if (huart->Instance == USART2) {
        UART_2.Load(1);
        UART2_IT_Handler();
    }
    #endif
    #ifdef UART3
    if (huart->Instance == USART3) {
        UART_3.Load(1);
        UART3_IT_Handler();
    }
    #endif
    #ifdef UART4
    if (huart->Instance == USART4) {
        UART_4.Load(1);
        UART4_IT_Handler();
    }
    #endif
}
