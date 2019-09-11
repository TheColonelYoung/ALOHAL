#include "cli.hpp"

#include "globals.hpp"

void CLI::Connect(UART *connection){
    serial_connection = connection;
    serial_connection->IRQ->Register(this, &CLI::Char_load);
    serial_connection->Send("\r\n");
}

void CLI::Start(){
    Register_command("help", "Print available commands or help for given command, example: help, help build", this, &CLI::Help);
    Register_command("build_info", "Print information about compilation as compiler version and date of compilation", this, &CLI::Build_info);
    New_line();
}

void CLI::Char_load(){
    string received_char = serial_connection->Read(1);
    if (static_cast<int>(received_char[0]) == 13){ // newline (screen - \r)
        Process_line();
        New_line();
        return;
    } else if (static_cast<int>(received_char[0]) == 127){ // backspace (screen - DEL)
        if (actual_line.length() > (line_opening.length() + filesystem_prefix.length())){
            serial_connection->Send("\r" + string(actual_line.length() + filesystem_prefix.length(),' '));
            actual_line.assign(actual_line.substr(0, actual_line.length() - 1));
        }
    } else if(isprint(static_cast<int>(received_char[0]))) {
        actual_line += received_char;
    }
    Redraw_line();
}

int CLI::Process_line(){
    size_t position = actual_line.find(line_opening);
    string cmd_line = actual_line.substr(position + line_opening.length(), actual_line.length() - position - line_opening.length());

    // only Enter is pressed
    if(cmd_line == ""){
        Print("\r\n");
        New_line();
        return 0;
    }

    // parse command line into vector of strings (first is command, next are arguments)
    vector<string> args;
    while ((position = cmd_line.find(" ")) != string::npos){
        args.emplace_back(cmd_line.substr(0,position));
        cmd_line.erase(0,position+1);
    }
    args.emplace_back(cmd_line);

    if (args.size() < 1){
        return -1;
    }
    Print("\r\r\n");

    for(auto &command:commands){
        if(args[0] == command->Get_command()){
            int ret = command->Invoke(args);

            actual_line.assign(line_opening);
            New_line();
            return ret;
        }
    }
    Print("Command \"" + args[0] + "\" was not found\r\n");
    return -1;
}

int CLI::Redraw_line(){
    serial_connection->Send("\r" + actual_line);
    return actual_line.length();
}

void CLI::Set_filesystem_prefix(const string prefix){
    filesystem_prefix = prefix;
    if(filesystem_prefix.back() == '/' && filesystem_prefix.length() > 1){
        filesystem_prefix.erase(filesystem_prefix.length()-1);
    }
    filesystem_prefix = "\u001b[34;1m" + filesystem_prefix + "\u001b[0m";
    New_line();
}

void CLI::New_line(){
    actual_line.assign(filesystem_prefix + line_opening);
    Redraw_line();
}

int CLI::Print(const string text){
    serial_connection->Send(text);
    return text.length();
}

int CLI::Help(vector<string> args){
    if(args.size() == 1){ // only help without parametr
        string output_line = "Available commands - " + to_string(commands.size()) + "\r\n";
        for(auto &cmd:commands){
            output_line += cmd->Get_command() + "\r\n";
        }
        Print(output_line);
    } else {    // help with additional command as parametr
        for(auto &cmd:commands){
            if (cmd->Get_command() == args[1]){
                Print(cmd->Get_command() + " - " + cmd->Get_help() + "\r\n");
                return commands.size();;
            }
        }
        Print("Parameter was not found\r\n");
    }
    return commands.size();
}

int CLI::Build_info(vector<string> args){
    string output = "Build date: " + string(__DATE__) + " " +  string(__TIME__) +"\r\n";
    output += "Compiler: arm-none-eabi-g++ \r\n";
    output += "Compiler version: "  + string(__VERSION__) + "\r\n";
    output += "C++ standard: "  + to_string(__cplusplus) + "\r\n";
    Print( output );
    return 0;
}

