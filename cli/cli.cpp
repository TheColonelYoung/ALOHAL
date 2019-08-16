#include "cli.hpp"

#include "globals.hpp"

void CLI::Connect(UART *connection){
    serial_connection = connection;
    serial_connection->IRQ->Register(this, &CLI::Char_load);
    serial_connection->Send("\r\n");
    serial_connection->Send("CLI connected \r\n");
}

void CLI::Start(){
    Register_command("help", "Print available commands or help for given command, example: help, help build", this, &CLI::Help);
    Register_command("build_info", "Print information about compilation as compiler version and date of compilation", this, &CLI::Build_info);
    actual_line.assign(line_opening);
    Redraw_line();
}

void CLI::Char_load(){
    string received_char = serial_connection->Read(1);
    if (static_cast<int>(received_char[0]) == 13){ //newline in `screen` \r
        Process_line();
    } else {
        actual_line += received_char;
        Redraw_line();
    }
}

int CLI::Process_line(){
    size_t position = actual_line.find(line_opening);
    string cmd_line = actual_line.substr(position+1, actual_line.length()-position-1);

    // only Enter is pressed
    if(cmd_line == ""){
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

    for(auto &command:commands){
        if(args[0] == command->Get_command()){
            //serial_connection->Send("\r\nStarting command: " + command->Get_command());
            return command->Invoke(args);
        }
    }
    Print("Command was not found\r\n");
    return -1;
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
        Print("Parameterwas not found\r\n");
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

