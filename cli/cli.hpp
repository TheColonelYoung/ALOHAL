#pragma once

#include <string>
#include <vector>

#include "uart/uart.hpp"
#include "command.hpp"

class CLI
{
private:
    UART *serial_connection = nullptr;
    string actual_line = "";
    const string line_opening = ">";

    vector<Command_base *> commands;

public:
    CLI() =default;

    void Connect(UART *connection);

    int Start();

    void Char_load();
    int Redraw_line();
    void New_line();
    int Print(string text);

    int Process_line();

    int Help(vector<string> args);
    int Build_info(vector<string> args);

    template <typename registrator_class>
    int Register_command(string command, string help, registrator_class &object, int (registrator_class::*method)(vector<string> args)){
        commands.emplace_back(new Command<registrator_class>(command, help, object, method));
        return 0;
    }

    template <typename registrator_class>
    int Register_command(string command, string help, registrator_class *object, int (registrator_class::*method)(vector<string> args)){
        commands.emplace_back(new Command<registrator_class>(command, help, object, method));
        return 0;
    }


};


