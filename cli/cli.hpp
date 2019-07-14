#pragma once

#include <string>

#include "uart/uart.hpp"


class CLI
{
private:
    UART *serial_connection;
    string actual_line = "";
    const string line_opening = ">";
public:
    CLI() =default;

    void Connect(UART &connection);

    int Start();

    void Char_load();
    int Redraw_line();
    void New_line();
    int Print(string text);
};


