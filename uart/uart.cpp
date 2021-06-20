#include "uart.hpp"

#include "device/device.hpp"
#include "rtos/thread.hpp"

UART::UART(UART_HandleTypeDef *UART_Handler_set){
    UART_Handler = UART_Handler_set;
    HAL_UART_Receive_IT(UART_Handler_set, UART_buffer_temp, 1);
    RX_buffer.reserve(20);
    TX_buffer.resize(buffer_size);
}

int UART::Send(string message){
    if (message.length() == 0) {
        return 0;
    }
    // Save message to buffer if UART is now busy
    Add_to_buffer(message);
    if (busy) {
        return TX_buffer.size();
    }
    // Send message and set UART as busy
    busy = true;
    string &text = TX_buffer[buffer_index_begin];
    HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) text.c_str(), text.length());
    return message.length();
}

unsigned short UART::Add_to_buffer(string &message){
    if (buffer_index_begin == ((buffer_index_end + 1) % buffer_size)) {
        return 0;
    }
    TX_buffer[buffer_index_end] = message;
    buffer_index_end = (buffer_index_end + 1) % buffer_size;
    return abs(buffer_index_begin - buffer_index_end - 1);
}

int UART::Send_poll(string message){
    HAL_UART_Transmit(UART_Handler, (unsigned char *) message.c_str(), message.length(), 200);
    return message.length();
}

int UART::Receive(){
    // If receiver buffer is full erase the last but one character in buffer
    if (RX_buffer.capacity() - 2 > RX_buffer.size()) {
        RX_buffer.push_back(UART_buffer_temp[0]);
    } else {
        RX_buffer[RX_buffer.size() - 1] = UART_buffer_temp[0];
    }
    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
    return 0;
}

void UART::Resend(){
    buffer_index_begin = (buffer_index_begin + 1) % buffer_size;
    if (buffer_index_begin != buffer_index_end) {
        string &message = TX_buffer[buffer_index_begin];
        HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) message.c_str(), message.length());
    } else {
        busy = false;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    #ifdef UART_1_EN
    if (huart->Instance == USART1) {
        device()->mcu->UART_1->Receive();
        device()->mcu->UART_1->IRQ->Notify();
        return;
    }
    #endif
    #ifdef UART_2_EN
    if (huart->Instance == USART2) {
        device()->mcu->UART_2->Receive();
        device()->mcu->UART_2->IRQ->Notify();
        return;
    }
    #endif
    #ifdef UART_3_EN
    if (huart->Instance == USART3) {
        device()->mcu->UART_3->Receive();
        device()->mcu->UART_3->IRQ->Notify();
        return;
    }
    #endif
    #ifdef UART_4_EN
    if (huart->Instance == USART4) {
        device()->mcu->UART_4->Receive();
        device()->mcu->UART_4->IRQ->Notify();
        return;
    }
    #endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    #ifdef UART_1_EN
    if (huart->Instance == USART1) {
        device()->mcu->UART_1->Resend();
        return;
    }
    #endif
    #ifdef UART_2_EN
    if (huart->Instance == USART2) {
        device()->mcu->UART_2->Resend();
        return;
    }
    #endif
    #ifdef UART_3_EN
    if (huart->Instance == USART3) {
        device()->mcu->UART_3->Resend();
        return;
    }
    #endif
    #ifdef UART_4_EN
    if (huart->Instance == USART4) {
        device()->mcu->UART_4->Resend();
        return;
    }
    #endif
}
