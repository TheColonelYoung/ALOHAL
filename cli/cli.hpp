#pragma once

#include <string>
#include <vector>

#include "uart/serial_line.hpp"
#include "command.hpp"
#include "history.hpp"

class Filesystem;

class CLI
{
private:
    /**
     * @brief Communication interface used for all CLI interaction
     */
    Serial_line *serial_connection = nullptr;

    /**
     * @brief Content of line on screen including FS prefix and line opening
     */
    string actual_line = "";

    /**
     * @brief   Last printed line used to determinated how much white space is needed for
     *              redrawing trailing printed character
     *          Without it adding white space: AB -> C => CB
     */
    string last_printed_line = "";

    /**
     * @brief Content which is inserted to every new line
     */
    const string line_opening = "\u001b[1m\u001b[32;1m>\u001b[0m ";

    /**
     * @brief Pointer to filesystem, which is connected to CLI
     */
    Filesystem* fs = nullptr;

    /**
     * @brief   Prefix before line opening, contains location inside filesystem, updated with every usage of command cd
     *          Used before line opening on new line
     */
    string filesystem_prefix = "";

    /**
     * @brief   List of command which can be used inside CLI
     *          Contains generic linux commands as ls, cd, pwd ...
     */
    vector<Command_base *> commands;

    /**
     * @brief   Pointer to object which add support for command history
     */
    CLI_history *command_history = nullptr;

    /**
     * @brief   Number of remaining characters of escape sequence
     *          If higher then 0, escape sequence is recording
     *          Now are only supported ^[A (arrow up) and ^[B (arrow down)
     *              for command history browsing
     */
    unsigned short escape_sequency_remaining = 0;

public:
    CLI() =default;

    /**
     * @brief Connect CLI to specified serial interface, this interface will be
     * used for comunication with user
     *
     * @param connection Serial communication interface (UART of USB CDC)
     */
    void Connect(Serial_line *connection);

    /**
     * @brief Enable lading characters from user, register basic CLI commands
     */
    void Start();

    /**
     * @brief   Load next available char from Serial_line, eventually start processing of line
     *          Called from IRQ Handler
     *
     */
    void Char_load();

    /**
     * @brief   Print given string to CLI to new line
     *          New line characters "\r\n" must be added by programer to end line
     *
     * @param text text to print on CLI screen
     * @return int Length of new line
     */
    int Print(string text);

    /**
     * @brief   Enable command history and listing in it for CLI
     *          Can be only used once, another usage has no effect
     *          Size of command history cannot be changed during run
     *
     * @param size_of_history   Number of commands which will be stored in history buffer
     * @return int              -1 if history already exists
     */
    int Enable_history(uint size_of_history);

    /**
     * @brief   Enable to run any executable from file system, executable cannot have name of any command
     *
     * @param fs Pointer to filesystem, from which will be executable loaded
     * @return int 0 if OK, 1 if filesystem is already set
     */
    int Enable_filesystem_executable(Filesystem* fs);

    /**
     * @brief   Save prefix which will be printed before line opening,
     *          This prefix means current location inside filesystem
     *          Called by filesystem everytime when 'cd' command is used
     *
     * @param prefix    Position in filesystem
     */
    void Set_filesystem_prefix(const string prefix);

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

private:

    /**
     * @brief Print new empty line to CLI
     *
     */
    void Clear_line();

    /**
     * @brief Print new string on actual line
     *
     * @return int Size of actual line
     */
    int Redraw_line();

    /**
     * @brief Set text to actual line, FS prefix and line opening is added
     *
     * @param text  Text to set
     * @return int  Length of line
     */
    int Set_line(string text);

    /**
     * @brief Process one input character of command line
     *
     * @return int Number of remaining characters in buffer
     */
    void Process_character();

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
     * @brief   Check command history avalability
     *
     * @return true     Command history function is available
     * @return false    Command history function is not available
     */
    bool History() { return command_history != nullptr;};


    int Autocomplete(string to_complete);
};
