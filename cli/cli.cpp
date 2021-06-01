#include "cli.hpp"

#include "filesystem/filesystem.hpp"
#include "device/device.hpp"
#include "globals.hpp"

void CLI::Connect(Serial_line *connection){
    serial_connection = connection;
    serial_connection->IRQ->Register(this, &CLI::Char_load);
    serial_connection->Send("\r\n");
}

void CLI::Start(){
    Register_command("help", "Print available commands or help for given command, example: help, help build", this, &CLI::Help);
    Register_command("build_info", "Print information about compilation as compiler version and date of compilation", this, &CLI::Build_info);
    Clear_line();
}

void CLI::Char_load(){
    while (serial_connection->Buffer_size() > 0) {
        Process_character();
    }
}

void CLI::Process_character(){
    char received_char = serial_connection->Read(1)[0];

    if (escape_sequency_remaining > 1) {
        escape_sequency_remaining--;
        return;
    }

    // Newline (picocom - \r)
    if (received_char == 13) {
        Process_line();
        Clear_line();
        return;
    }
    // Escape sequence -> skip next character [, after that use arrow sign
    else if (received_char == 27) {
        escape_sequency_remaining = 2;
        return;
    }
    // Backspace (screen - DEL)
    else if (received_char == 127) { // backspace (screen - DEL)
        if (actual_line.length() > (line_opening.length() + line_prefix.length())) {
            actual_line = actual_line.substr(0, actual_line.length() - 1);
        }
    }
    // Handle escape sequence for arrows
    else if (escape_sequency_remaining == 1) {
        if (command_history) {
            // History arrow up
            if (received_char == 65) {
                Set_line(command_history->Up());
            }
            // History arrow down
            else if (received_char == 66) {
                Set_line(command_history->Down());
            }
        }
        escape_sequency_remaining = 0;
    }
    // Autocomplete
    else if (received_char == 9) {
        // If nothing on line skip autocomplete
        if ((actual_line.back() != ' ') or (actual_line != line_prefix + line_opening)) {
            // Autocomplete for string between last space and and of line
            Autocomplete(actual_line.substr(actual_line.find_last_of(" ") + 1, actual_line.length()));
        }
    }
    // Any other printable character
    else if (isprint(received_char)) {
        actual_line += string(1, received_char);
    }
    Redraw_line();
} // CLI::Process_character

int CLI::Process_line(){
    size_t position = actual_line.find(line_opening);
    string cmd_line = actual_line.substr(position + line_opening.length(), actual_line.length() - position - line_opening.length());

    // only Enter is pressed
    if (cmd_line == "") {
        Print("\r\n");
        Clear_line();
        if (History()) {
            command_history->Reset_pointer();
        }
        return 0;
    }

    // Add new record to history if is enabled
    if (History()) {
        command_history->Update(cmd_line);
    }

    // parse command line into vector of strings (first is command, next are arguments)
    vector<string> args;
    while ((position = cmd_line.find(" ")) != string::npos) {
        string cmd = cmd_line.substr(0, position);
        if (cmd.find_first_not_of(' ') != std::string::npos) {
            args.emplace_back(cmd);
        }
        cmd_line.erase(0, position + 1);
    }
    args.emplace_back(cmd_line);

    if (args.size() < 1) {
        return -1;
    }
    Print("\r\r\n");

    // Handle input for foreground application
    if (fs and foreground_application) {
        // Insert name of app as first argument to adhere to conventions
        args.insert(args.begin(), foreground_application->Name());
        Process_foreground_application(args);
        return 0;
    }

    // Run registred command
    for (auto &command:commands) {
        if (args[0] == command->Get_command()) {
            int ret = command->Invoke(args);
            actual_line = line_opening;
            Clear_line();
            return ret;
        }
    }

    // Run executable from filesystem if exists
    if (fs) {
        string path = args[0];
        unsigned int position = path.find_last_of("/");
        if (position != string::npos) {
            args[0] = path.substr(position + 1);
        }
        return fs->Execute(path, args);
    }

    Print("Command \"" + args[0] + "\" was not found\r\n");
    return -1;
} // CLI::Process_line

bool CLI::Start_foreground_application(string path_to_application){
    if (fs and fs->Entry_exists(path_to_application)) {
        if (fs->Entry_type(path_to_application) == FS_entry::Type::Executable) {
            foreground_application = static_cast<Executable *>(fs->Get_entry(path_to_application));
            Set_line_prefix("$" + foreground_application->Name());
            return true;
        }
    }
    return false;
}

void CLI::Stop_foreground_application(){
    foreground_application = nullptr;
    if (fs) {
        // Refresh line opening for filesystem when exiting foreground application
        fs->Set_location(".");
    }
}

void CLI::Process_foreground_application(vector<string> args){
    if (args[1] == "exit") {
        Stop_foreground_application();
    } else {
        foreground_application->Run(args);
    }
}

int CLI::Enable_filesystem_executable(Filesystem *fs){
    int ret = 0;
    if (this->fs != nullptr) {
        ret = 1;
    }
    this->fs = fs;
    return ret;
}

int CLI::Redraw_line(){
    // Actual_line is text which will be now printed
    int missing_spaces = last_printed_line.length() - actual_line.length();
    if (missing_spaces > 0) {
        Print("\r" + string(last_printed_line.length(), ' ') + "\r" + actual_line);
    } else {
        Print("\r" + actual_line);
    }
    last_printed_line = actual_line;
    return actual_line.length();
}

int CLI::Set_line(string text){
    actual_line = line_prefix + line_opening + text;
    return actual_line.size();
}

void CLI::Set_line_prefix(const string prefix){
    line_prefix = prefix;
    if (line_prefix.back() == '/' && line_prefix.length() > 1) {
        line_prefix.erase(line_prefix.length() - 1);
    }
    line_prefix = "\u001b[34;1m" + line_prefix + "\u001b[0m";
    Clear_line();
}

void CLI::Clear_line(){
    actual_line = line_prefix + line_opening;
    Redraw_line();
}

int CLI::Print(const string text){
    serial_connection->Send(text);
    return text.length();
}

int CLI::Enable_history(uint size_of_history){
    if (command_history != nullptr) {
        return -1;
    }
    command_history = new CLI_history(size_of_history);
    return 0;
}

int CLI::Help(vector<string> args){
    if (args.size() == 1) { // only help without parametr
        string output_line = "Available commands - " + to_string(commands.size()) + "\r\n";
        for (auto &cmd:commands) {
            output_line += cmd->Get_command() + "\r\n";
        }
        Print(output_line);
    } else { // help with additional command as parametr
        for (auto &cmd:commands) {
            if (cmd->Get_command() == args[1]) {
                Print(cmd->Get_command() + " - " + cmd->Get_help() + "\r\n");
                return commands.size();

                ;
            }
        }
        Print("Parameter was not found\r\n");
    }
    return commands.size();
}

int CLI::Build_info(vector<string> args){
    string output = "Build date: " + string(__DATE__) + " " + string(__TIME__) + "\r\n";
    output += "Compiler: arm-none-eabi-g++ \r\n";
    output += "Compiler version: " + string(__VERSION__) + "\r\n";
    output += "C++ standard: " + to_string(__cplusplus) + "\r\n";
    Print(output);
    return 0;
}

int CLI::Autocomplete(string to_complete){
    // Print("To complete: " + to_complete + "\r\n");

    if (!fs) {
        // Cannot perform autocomplete, FS is not available
        return -1;
    }

    // Get names of entries in actual location
    vector<string> all_names = fs->Current_location_content();

    vector<string> candidate_names;

    for (auto &entry:all_names) {
        if (entry.find(to_complete) == 0) {
            candidate_names.push_back(entry);
        }
    }

    if (candidate_names.size() > 1) {
        Print("\r\n");
        for (auto &candidate:candidate_names) {
            Print(candidate + "\r\n");
        }
    } else if (candidate_names.size() == 1) {
        actual_line += candidate_names.front().substr(to_complete.length(), candidate_names.front().length());
    }

    return 0;
}
