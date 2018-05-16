#include "protocol.hpp"

extern Eyrina Eyrina_platform;

E_protocol::E_protocol(UART *input) : UART_input(input){ }

uint8_t board_command_table[][2] = { };

uint8_t motor_command_table[][2] = {
    { 1, 2 }, { 2, 3 }, { 3, 2 }, { 8, 1 }
};

uint8_t LED_driver_command_table[][2] = {
    { 0, 3 }, { 1, 2 }, { 2, 3 }, { 3, 2 }
};

uint8_t PWM_driver_command_table[][2] = {
    { 0, 2 }, { 3, 2 }
};

void E_protocol::Initial_byte(){
    if (wait_for == 0) {
        command_seq.push_back(UART_input->RX_buffer[0]); // save first part of command

        uint8_t receiver = (UART_input->RX_buffer[0] & 0b11110000) >> 4; // get receiver and first part of command
        uint8_t command  = UART_input->RX_buffer[0] & 0b00001111;
        wait_for = Length_finder(receiver, command);
    } else {                                             // waiting for other bytes which will be saved to buffer
        command_seq.push_back(UART_input->RX_buffer[0]); // save data as next part of command
        wait_for--;
    }

    if (wait_for == 0) {
        Decode_command(); // send buffer for decoding of command
        command_seq.clear();
    }

    UART_input->Clear_buffer();
}

int E_protocol::Length_finder(uint8_t receiver, uint8_t command){
    if (receiver == 0) {
        TABLE_SCAVENGER(board_command_table);
    } else if (receiver >= 1 && receiver <= 6) {
        TABLE_SCAVENGER(motor_command_table);
    } else if (receiver >= 7 && receiver <= 8) {
        TABLE_SCAVENGER(LED_driver_command_table);
    } else if (receiver >= 9 && receiver <= 11) {
        TABLE_SCAVENGER(PWM_driver_command_table);
    }
    return 0;
}

void E_protocol::Decode_command(){
    uint8_t receiver = (command_seq[0] & 0b11110000) >> 4;
    uint8_t command  = command_seq[0] & 0b00001111;

    if (receiver == 0) {
        Perform_board_command(command);
    } else if (receiver >= 1 && receiver <= 6) {
        Perform_motor_command(receiver, command);
    } else if (receiver >= 7 && receiver <= 8) {
        Perform_LED_driver_command(receiver, command);
    } else if (receiver >= 9 && receiver <= 11) {
        Perform_PWM_command(receiver, command);
    }
}

void E_protocol::Perform_board_command(uint8_t command){
    if (command == 0) { // response OK
        Response(1);
    } else if (command == 7) { // toggle signal LED
        Eyrina_platform.DBG_LED_2.Toggle();
        Response(1);
    }
}

void E_protocol::Perform_motor_command(uint8_t receiver, uint8_t command){
    if (command == 1) { // Set speed
        Response(2);
    } else if (command == 2) { // Set shift
        uint sign = (command_seq[1] & 0b01000000) >> 6;

        UART_2.Send(sign);

        sign = (sign * -2) +1;

        UART_2.Send(sign);
        UART_2.Send("\r\n");
        // bool force = (command_seq[1] & 0b10000000) >> 7;
        int value = command_seq[3] | (command_seq[2] << 8) | (command_seq[1] & 0b00001111) << 16;

        Eyrina_platform.Move_axis(static_cast<Eyrina::Axis>(receiver - E_PROTOCOL_MOTOR_COM_OFFSET), sign * value);

        Response(2);
    } else if (command == 8) { // change microstepping
        // NOTE: not enabled
        Response(2);
    } else if (command == 5) { // Rotation to left
        // NOTE: Disabled, too dangerous without endstops
        Response(2);
    } else if (command == 6) {
        Eyrina_platform.Stop_axis(static_cast<Eyrina::Axis>(receiver - E_PROTOCOL_MOTOR_COM_OFFSET));
        Response(2);
    } else if (command == 7) { // Rotation to right
        // NOTE: Disabled, too dangerous without endstops
        Response(2);
    }
}

void E_protocol::Perform_LED_driver_command(uint8_t receiver, uint8_t command){
    if (command == 0) {
        float value = (command_seq[2] | ((command_seq[1] & 0b00001111) << 8));
        Eyrina_platform.LED_driver_current(receiver - E_PROTOCOL_LED_DRIVER_COM_OFFSET, value * 1000);
    }
    Response(3);
}

void E_protocol::Perform_PWM_command(uint8_t receiver, uint8_t command){
    if (command == 0) {
        float value = (command_seq[2] | ((command_seq[1] & 0b00111111) << 8)) / 10;
        Eyrina_platform.PWM(receiver - E_PROTOCOL_PWM_COM_OFFSET, value);
    } else if (command == 3) {
        float value = (command_seq[2] | ((command_seq[1] & 0b00111111) << 8)) / 10;
        Eyrina_platform.PWM_limit(receiver - E_PROTOCOL_PWM_COM_OFFSET, value);
    }
    Response(4);
}

bool E_protocol::Response(int return_code){
    if (response_en) {
        UART_input->Send(return_code);
    }
    return response_en;
}
