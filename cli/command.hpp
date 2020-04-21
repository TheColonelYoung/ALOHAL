#pragma once

#include <string>
#include <vector>

using namespace std;

/**
 * @brief   Base class that allows to store all derivated Commands in one vector
 *          Event when Commands tamplates are based on different classes
 */
class Command_base
{
private:
public:
    Command_base() = default;
    ~Command_base() = default;

    virtual const int Invoke(vector<string> &args) = 0;
    virtual const string Get_command() = 0;
    virtual const string Get_help() = 0;
};

/**
 * @brief   Contain one command for CLI
 *          Command can be executed from every place in CLi
 */
template<typename template_class>
class Command : public Command_base{
private:
    /**
     * @brief Name of command, like 'ls'
     */
    string command;

    /**
     * @brief Helping string, description of command
     */
    string help;

    /**
     * @brief Pointer to object which is responsible for command
     */
    template_class * object_ptr = nullptr;

    /**
     * @brief Pointer to method of object, which is performing a command
     */
    int (template_class::*method_pointer)(vector<string> args);

    /**
     * @brief Pointer to function which perform command
     */
    int (*function)(vector<string> args) = nullptr;

public:
    /**
     * @brief Construct a new Command object
     */
    Command() =default;
    ~Command() =default;

    /**
     * @brief   Construct a new Command object based on object and his method
     *
     * @param command   Name of command, like 'ls'
     * @param help      Helping string, description of command
     * @param object    Object which is responsible for command
     * @param method    Method of object which is executed
     */
    Command(string command, string help, template_class *object, int (template_class::*method)(vector<string> args))
        :command(command), help(help), object_ptr(object), method_pointer(method)
    {}

    /**
     * @brief   Construct a new Command object based on function
     *
     * @param command   Name of command, like 'ls'
     * @param help      Helping string, description of command
     * @param function_pointer_set  Pointer to function which perform command
     */
    Command(string command, string help,int (*function_pointer_set)(vector<string> args))
        :command(command), help(help), function(function)
    {}

    /**
     * @brief Invoke command encapsulated inside command
     *
     * @param args          Arguments of command
     * @return const int    Return code of command
     */
    const int Invoke(vector<string> &args) final override {
        if (function) {
            return (*(function))(args);
        } else if (object_ptr){
            return (*object_ptr.*method_pointer)(args);
        }
        return -1;
    }

    /**
     * @brief Gettor for command name
     *
     * @return const string Command name
     */
    inline const string Get_command() final override {return command;};

    /**
     * @brief Gettor for command help string
     *
     * @return const string Command help
     */
    inline const string Get_help() final override {return help;};
};



