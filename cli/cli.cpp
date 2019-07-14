#include "cli.hpp"

void CLI::Connect(UART &connection){
    serial_connection = &connection;
    connection.IRQ.Register(this, &CLI::Char_load);
    //device.mcu.UART_2.IRQ.Register(device.cli, &CLI::Char_load);
    connection.Send("CLI connected \r\n");
}

int CLI::Start(){
    actual_line.assign(line_opening);
    Redraw_line();
    return 0;
}

void CLI::Char_load(){
    string received_char = serial_connection->Read(1);
    if (static_cast<int>(received_char[0]) == 13){ //newline in `screen` \r
        Pin('B',3).Toggle();
        New_line();
    } else {
        actual_line += received_char;
        Redraw_line();
    }
}

int CLI::Redraw_line(){
    serial_connection->Send("\r" + actual_line);
    return actual_line.length();
}

void CLI::New_line(){
    actual_line.assign(line_opening);
    serial_connection->Send("\r\n");
    Redraw_line();
}

int CLI::Print(string text){
    actual_line.assign(line_opening);
    serial_connection->Send("\r\n" + text);
    Redraw_line();
    return text.length();
}
