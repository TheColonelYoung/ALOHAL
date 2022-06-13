#include "uart.hpp"

#include "device/device.hpp"

UART::UART(UART_HandleTypeDef *UART_Handler_set){
    UART_Handler = UART_Handler_set;

    HAL_UART_Receive_IT(UART_Handler_set, UART_buffer_temp, 1);
}

int UART::Send(string message){
    if (message.length() == 0) {
        return 0;
    }
    // Save message to buffer if UART is now busy
    if (busy) {
        TX_buffer.emplace_back(message);
        return TX_buffer.size();
    }
    // Send message and set UART as busy
    busy = true;
    TX_buffer.emplace_back(message);
    HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) TX_buffer.front().c_str(), TX_buffer.front().length());
    return message.length();
}

int UART::Receive(){
    RX_buffer.push_back(UART_buffer_temp[0]);
    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
    return 0;
}

int UART::Resend(){
    TX_buffer.erase(TX_buffer.begin()); // Erase message which transfer is complete
    if (TX_buffer.size() > 0) {         // Send next message in line
        HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) TX_buffer.front().c_str(), TX_buffer.front().length());
    } else  { // Now is UART unoccupied
        busy = false;
    }
    return TX_buffer.size();
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
    #ifdef LPUART_1_EN
    if (huart->Instance == LPUART1) {
        device()->mcu->LPUART_1->Receive();
        device()->mcu->LPUART_1->IRQ->Notify();
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
    #ifdef LPUART_1_EN
    if (huart->Instance == LPUART1) {
        device()->mcu->LPUART_1->Resend();
        return;
    }
    #endif
}
