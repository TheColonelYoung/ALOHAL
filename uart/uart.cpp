#include "uart.hpp"

unsigned char UART_1_buffer_temp[10];
unsigned char UART_2_buffer_temp[10];
unsigned char UART_3_buffer_temp[10];
unsigned char UART_4_buffer_temp[10];

UART::UART(UART_HandleTypeDef *UART_Handler_set){
    UART_Handler = UART_Handler_set;

    #ifdef UART_1_EN
    if (UART_Handler->Instance == USART1) {
        UART_buffer_temp = UART_1_buffer_temp;
    }
    #endif

    #ifdef UART_2_EN
    if (UART_Handler->Instance == USART2) {
        UART_buffer_temp = UART_2_buffer_temp;
    }
    #endif

    #ifdef UART_3_EN
    if (UART_Handler->Instance == USART3) {
        UART_buffer_temp = UART_3_buffer_temp;
    }
    #endif

    #ifdef UART_4_EN
    if (UART_Handler->Instance == USART4) {
        UART_buffer_temp = UART_4_buffer_temp;
    }
    #endif

    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
}

int UART::Send(string message){
    if (HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) message.c_str(), message.length()) != HAL_OK) { }
    return message.length();
}

int UART::Send(int number){
    string message = to_string(number);
    HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) message.c_str(), message.length());
    return message.length();
}

int UART::Send(uint number){
    return Send((int) number);
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
    UART1.Send(UART1.UART_buffer);
    UART1.Clear_buffer();
}

void UART2_IT_Handler(){ }

void UART3_IT_Handler(){
    // UART_1.Send(UART_3.UART_buffer.length());
    // ESP_Receive();
}

void UART4_IT_Handler(){ }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    #ifdef UART_1_EN
    if (huart->Instance == USART1) {
        UART1.Load(1);
        UART1_IT_Handler();
    }
    #endif
    #ifdef UART_2_EN
    if (huart->Instance == USART2) {
        UART2.Load(1);
        UART2_IT_Handler();
    }
    #endif
    #ifdef UART_3_EN
    if (huart->Instance == USART3) {
        UART3.Load(1);
        UART3_IT_Handler();
    }
    #endif
    #ifdef UART_4_EN
    if (huart->Instance == USART4) {
        UART4.Load(1);
        UART4_IT_Handler();
    }
    #endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    // Pin('C',14).Toggle();
}
