#include "protocol.hpp"

extern Eyrina Eyrina_platform;

E_protocol::E_protocol(UART *input) : UART_input(input){ }

uint8_t board_command_table[][2] = { };

uint8_t motor_command_table[][2] = {
    { 1, 2 }, { 2, 3 }, { 3, 2 }, { 8, 1 }
};

uint8_t LED_driver_command_table[][2] = {
    { 2, 2 }
};

uint8_t PWM_driver_command_table[][2] = {
    { 2, 2 }
};

void E_protocol::Initial_byte(){
    if (wait_for == 0) {
        command_seq.push_back(UART_input->RX_buffer[0]); // save first part of command

        uint8_t receiver = (UART_input->RX_buffer[0] & 0b11110000) >> 4; // get receiver and first part of command
        uint8_t command  = UART_input->RX_buffer[0] & 0b00001111;

        wait_for = Length_finder(receiver, command);
        UART_input->Send(wait_for);
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
        Perform_motor_command(command);
    } else if (receiver >= 7 && receiver <= 8)  {
        Perform_LED_driver_command(command);
    } else if (receiver >= 9 && receiver <= 11) {
        Perform_PWM_command(command);
    }
}

void E_protocol::Perform_board_command(uint8_t command){
    if (command == 0) { // response OK
        Response(1);
    } else if (command == 7) { // toggle signal LED
        Eyrina_platform.DBG_LED_1.Toggle();
        Response(1);
    }
}

void E_protocol::Perform_motor_command(uint8_t command){
    if (command == 1) { // set speed
        // int value  = command_seq[2] | (command_seq[1] & 0b00001111) << 8;
        // bool sign  = (command_seq[1] & 0b01000000) >> 6;
        // bool force = (command_seq[1] & 0b10000000) >> 7;
        // SM_set_speed(value,sign,force);
        Response(1);
    } else if (command == 2) { // set shift
        bool sign = (command_seq[1] & 0b01000000) >> 6;
        sign *= -1;
        // bool force = (command_seq[1] & 0b10000000) >> 7;
        int value = command_seq[3] | (command_seq[2] << 8) | (command_seq[1] & 0b00001111) << 16;

        Eyrina_platform.Move_axis(Eyrina::Axis::X, sign * value);

        Response(1);
    } else if (command == 8) { // change microstepping
        //
        Response(1);
    } else if (command == 5) {
        // SM_rot_to_left();
        Response(1);
    } else if (command == 6) {
        // SM_stop();
        Response(1);
    } else if (command == 7) {
        // SM_rot_to_right();
        Response(1);
    }
}

void E_protocol::Perform_LED_driver_command(uint8_t command){
    if (command == 2) {
        /*
         * float value = (command_seq[2] | ((command_seq[1] & 0b00001111) << 8)) / 10.0;
         * if (receiver == 8) {
         *  LED_Driver_stash[1].Set_value(value);
         *  // DAC_set_value(2,value/100.0);
         * }
         * LED_Driver_stash[receiver - 7].Set_value(value);
         */
    }
}

void E_protocol::Perform_PWM_command(uint8_t command){
    if (command == 2) {
        /*
         * int value = (command_seq[2] | ((command_seq[1] & 0b00001111) << 8));
         * // PWM_stash[receiver - 9].duty_cycle = value;
         * if (value > 500) {
         *  value = 500;
         * }
         *
         * TIM_OC_InitTypeDef sConfigOC;
         * sConfigOC.Pulse      = value;
         * sConfigOC.OCMode     = TIM_OCMODE_PWM1;
         * sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
         * sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
         * HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
         * HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
         */
    }
}

bool E_protocol::Response(int return_code){
    if (response_en) {
        UART_input->Send(return_code);
    }
    return response_en;
}
