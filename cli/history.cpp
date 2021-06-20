#include "history.hpp"

CLI_history::CLI_history(uint size) :
    size(size){
    cmd_history.resize(size, "");
}

string CLI_history::Up(){
    if (pointer == end){ // End of history
        return cmd_history[end];
    } else {
        string command = cmd_history[pointer];
        pointer = (pointer - 1);
        if (pointer == -1){
            pointer = size-1;
        } else {
            pointer = pointer % size;
        }
        return command;
    }
}

string CLI_history::Down(){
    if (pointer == begin){ // Begining of history
        return "";
    } else {
        pointer = (pointer + 1) % size;
        return cmd_history[pointer];
    }
}

int CLI_history::Update(string new_command){
    // Initialization
    if (cmd_history[begin] == ""){
        cmd_history[begin] = new_command;
    } else {
        // Skip update if last command is same as new_command
        if (new_command == cmd_history[begin]){
            pointer = begin;
            return 1;
        }
        begin = (begin + 1) % size;
        if (begin == end) {
            end = (begin + 1) % size;
        }
        pointer = begin;
        cmd_history[begin] = new_command;
    }
    return 0;
}

void CLI_history::Reset_pointer(){
    pointer = begin;
}
