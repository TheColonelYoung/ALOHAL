#pragma once

#include <string>
#include <vector>

using namespace std;

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

template<typename template_class>
class Command : public Command_base{
private:
    string command;
    string help;

    template_class * object_ptr = nullptr;
    int (template_class::*method_pointer)(vector<string> args);

    int (*function)(vector<string> args) = nullptr;

public:
    Command() =default;
    ~Command() =default;

    Command(string command, string help, template_class *object, int (template_class::*method)(vector<string> args))
        :command(command), help(help), object_ptr(object), method_pointer(method)
    {}

    Command(string command, string help,int (*function_pointer_set)(vector<string> args))
        :command(command), help(help), function(function)
    {}

    const int Invoke(vector<string> &args) override {
        if (function) {
            return (*(function))(args);
        } else if (object_ptr){
            return (*object_ptr.*method_pointer)(args);
        }
        return -1;
    }

    inline const string Get_command() override {return command;};
    inline const string Get_help() override {return help;};
};



