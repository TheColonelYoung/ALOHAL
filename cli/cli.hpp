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

    /**
     * @brief Connect CLI to specified serial interface, this interface will be
     * used for comunication with user
     *
     * @param connection Serial communication interface (UART of USB CDC)
     */
    void Connect(UART *connection);

    /**
     * @brief Enable lading characters from user, register basic CLI commands
     */
    void Start();

    /**
     * @brief Load next available char from UART, eventually start processing of line
     *
     */
    void Char_load();

    /**
     * @brief Print new string on actual line
     *
     * @return int Size of actual line
     */
    int Redraw_line();

    /**
     * @brief Print new empty line to CLI
     *
     */
    void New_line();

    /**
     * @brief Print given string to CLI to new line
     *
     * @param text text to print on CLI screen
     * @return int Length of new line
     */
    int Print(string text);

    /**
     * @brief Process line (after entering enter), run command
     *
     * @return int Status of operation
     */
    int Process_line();

    /**
     * @brief CLI command, show available command or help for commands
     *
     * @param args Arguments on CLI for command
     * @return int Number of commands available
     */
    int Help(vector<string> args);
    /**
     * @brief CLI command, show information about compilation settings and date
     *
     * @param args Arguments on CLI for command
     * @return int Nothing
     */
    int Build_info(vector<string> args);

    /**
     * @brief Register method of some object as CLI command
     *
     * @tparam registrator_class    Class of object, which is making registration
     * @param command   Name of command
     * @param help      Help for command
     * @param object    Pointer to object on which will be method invocated
     * @param method    Method of class which will be invocated
     */
    template <typename registrator_class>
    void Register_command(string command, string help, registrator_class *object, int (registrator_class::*method)(vector<string> args)){
        commands.emplace_back(new Command<registrator_class>(command, help, object, method));
    }





};


