#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif

#include <vector>

#include "globals.hpp"
#include "uart/uart.hpp"
#include "dac/DAC.hpp"

using namespace std;

#define E_PROTOCOL_MAX_LENGTH 4

#define E_PROTOCOL_MOTOR_COM_OFFSET 1
#define E_PROTOCOL_LED_DRIVER_COM_OFFSET 7
#define E_PROTOCOL_PWM_COM_OFFSET 9

#define TABLE_SCAVENGER(table) \
    for (uint i = 0; i < ARRAY_SIZE(table); i++) { \
    if (table[i][0] == command) { \
        return table[i][1]; } }


class E_protocol {
    int wait_for = 0;

    bool response_en = false;

    UART *UART_input;

    vector<uint8_t> command_seq;

public:

    E_protocol() = default;
    E_protocol(UART *input);

    void Initial_byte();    //Decide initial byte of communication
    int Length_finder(uint8_t receiver, uint8_t command);   //Determine length of whole command
    void Decode_command();  //Read command after is complete and start performing it

    //This method perform completed command
    void Perform_board_command(uint8_t command);
    void Perform_motor_command(uint8_t receiver, uint8_t command);
    void Perform_LED_driver_command(uint8_t receiver, uint8_t command);
    void Perform_PWM_command(uint8_t receiver, uint8_t command);

    bool Response(int return_code);
};

#include "Eyrina/Eyrina.hpp"

#endif // ifndef EYRINA_HPP
