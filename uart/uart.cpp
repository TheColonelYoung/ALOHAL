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

    HAL_UART_Receive_IT(UART_Handler_set, UART_buffer_temp, 1);
}

int UART::Send(string message){
    if (busy) {
        TX_buffer.push_back(message);
        return TX_buffer.size();
    }
    busy = true;
    TX_buffer.push_back(message);
    HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) TX_buffer.front().c_str(), TX_buffer.front().length());
    return message.length();
}

int UART::Load(){
    UART_buffer.push_back(UART_buffer_temp[0]);
    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
    return 0;
}

int UART::Clear_buffer(){
    uint length = UART_buffer.length();
    UART_buffer = "";
    return length;
}

int UART::Resend(){
    TX_buffer.erase(TX_buffer.begin()); //Erase message which transfer is complete
    if (TX_buffer.size() > 0) { //Send next message in line
        HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) TX_buffer.front().c_str(), TX_buffer.front().length());
    }else{  //Now is UART unoccupied
        busy = false;
    }
    return TX_buffer.size();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    #ifdef UART_1_EN
    if (huart->Instance == USART1) {
        UART_1.Load();
        UART_1.IRQ.Notify();
        return;
    }
    #endif
    #ifdef UART_2_EN
    if (huart->Instance == USART2) {
        UART_2.Load();
        UART_2.IRQ.Notify();
        return;
    }
    #endif
    #ifdef UART_3_EN
    if (huart->Instance == USART3) {
        UART_3.Load();
        UART_3.IRQ.Notify();
        return;
    }
    #endif
    #ifdef UART_4_EN
    if (huart->Instance == USART4) {
        UART_4.Load();
        UART_4.IRQ.Notify();
        return;
    }
    #endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    #ifdef UART_1_EN
    if (huart->Instance == USART1) {
        UART_1.Resend();
        return;
    }
    #endif
    #ifdef UART_2_EN
    if (huart->Instance == USART2) {
        UART_2.Resend();
        return;
    }
    #endif
    #ifdef UART_3_EN
    if (huart->Instance == USART3) {
        UART_3.Resend();
        return;
    }
    #endif
    #ifdef UART_4_EN
    if (huart->Instance == USART4) {
        UART_4.Resend();
        return;
    }
    #endif
}
