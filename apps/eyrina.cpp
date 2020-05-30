#include "eyrina.hpp"

Eyrina::Eyrina() :
    Application("eyrina")
{ }

int Eyrina::Run(vector<string> &args){
    Log(Log_levels::Debug, "Eyrina Run");
    if (args.size() > 0){
        args.erase(args.begin());
    }
    Parse(args);
    return 0;
}

int Eyrina::Init(){
    Log(Log_levels::Debug, "Eyrina Init");
    return 0;
}

int Eyrina::Parse(vector<string> &gcode){
    
    string command = "";
    
    for (auto &section:gcode) {
        command += section + " ";
    }
    Log(Log_levels::Debug, "Command: " + command);
    
    // Save command name and removes it from arguments
    string command_tag = gcode[0];
    if (gcode.size() > 0){
        gcode.erase(gcode.begin());
    }

    // Create parameters and flags from command arguments
    vector<char> flags;
    map<char, double> params;

    for (auto &param:gcode) {
        if (param.length() == 1) {
            // Flags F
            flags.emplace_back(param[0]);
            Log(Log_levels::Debug, param.substr(0, 1));
        } else {
            // Params X-1.2
            double param_value = stod(param.substr(1, param.length() - 1));
            params.emplace(param[0], param_value);
            Log(Log_levels::Debug, param.substr(0, 1) + ":" + to_string(stod(param.substr(1, param.length() - 1))));
        }
    }
    
    // Execute validation and receive pointer to g-code execution method
    const gcode_settings *g_code_method = nullptr;
    
    int ret_code = Validation(command_tag, params, flags, g_code_method);
    
    if(ret_code){
        // Invalid command
        return ret_code;
    } else {
        // Execute method of g-code from command settings
        return (*this.*(*g_code_method).method)(params,flags);
    }
}

int Eyrina::Validation(string &command, map<char, double> &params, vector<char> &flags, const gcode_settings *&g_code_method){
    
    // Checks if commands tag exists in defined commands
    auto iterator = find_if(g_code_commands.begin(), g_code_commands.end(), 
        [&command] (const pair<string, gcode_settings> &g_code_command){
            return g_code_command.first == command;
        }
    );
    
    if (iterator == g_code_commands.end()){
        //Command does not exists
        Log(Log_levels::Debug, "Invalid command");
        return -1;
    }

    const gcode_settings gcode_structure = (*iterator).second;
    
    // Check if command has valid parameters
    for(auto &param:params){
        auto position = find(gcode_structure.params.begin(), gcode_structure.params.end(), param.first);
        if (position == gcode_structure.params.end()){
            // Param does not exist
            Log(Log_levels::Debug, "Invalid parameter");
            return -2;
        }
    }
    
    // Check if command has valid flags
    for(auto &flag:flags){
        auto position = find(gcode_structure.flags.begin(), gcode_structure.flags.end(), flag);
        if (position == gcode_structure.flags.end()){
            // Flag does not exist
            Log(Log_levels::Debug, "Invalid flag");
            return -3;
        }
    }
    
    Log(Log_levels::Debug, "Command is valid");
    
    g_code_method = &gcode_structure;
    
    return 0;
}

int Eyrina::Add_axis(char axis_name, Motion_axis *new_axis){
    if (axis.count(axis_name)){
        Log(Log_levels::Error, "Axis with this identifier already exists");
        return -1;
    }
    axis.insert({axis_name, new_axis});
    return 0;
}

int Eyrina::G_code_G0(map<char, double> &params, vector<char> &flags){
    
    return 0;
}

int Eyrina::G_code_G1(map<char, double> &params, vector<char> &flags){
    int moving_axis = 0;
    for(auto &axis_command:params){
        // First is tag of axis, second is distance to move
        if(axis.find(axis_command.first) != axis.end()){
            Log(Log_levels::Notice, "Homing axis " + axis_command.first);
            axis[axis_command.first]->Move(axis_command.second);
            moving_axis += 1;
        }
    }
    return moving_axis;
}

int Eyrina::G_code_G28(map<char, double> &params, vector<char> &flags){
    if(flags.size() == 0){
        Log(Log_levels::Notice, "Homing all axis");
        for (auto &elem:axis){
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        for(auto &axis_label:flags){
            if(axis.find(axis_label) != axis.end()){
                Log(Log_levels::Notice, "Homing axis " + axis_label);
                axis[axis_label]->GoHome();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int Eyrina::G_code_M0(map<char, double> &params, vector<char> &flags){
    if(flags.size() == 0){
        Log(Log_levels::Notice, "Soft stoping all axis");
        for (auto &elem:axis){
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        for(auto &axis_label:flags){
            if(axis.find(axis_label) != axis.end()){
                Log(Log_levels::Notice, "Soft stoping axis " + axis_label);
                axis[axis_label]->Sleep();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int Eyrina::G_code_M10(map<char, double> &params, vector<char> &flags){
    if(flags.size() == 0){
        Log(Log_levels::Notice, "Disabling all axis");
        for (auto &elem:axis){
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        for(auto &axis_label:flags){
            if(axis.find(axis_label) != axis.end()){
                Log(Log_levels::Notice, "Disabling axis " + axis_label);
                axis[axis_label]->Sleep();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int Eyrina::G_code_E1(map<char, double> &params, vector<char> &flags){
    
    return 0;
}

int Eyrina::G_code_E2(map<char, double> &params, vector<char> &flags){
    
    return 0;
}

int Eyrina::G_code_E3(map<char, double> &params, vector<char> &flags){
    
    return 0;
}

int Eyrina::G_code_R0(map<char, double> &params, vector<char> &flags){
    Log(Log_levels::Notice, "Emergency stop");
    for (auto &elem:axis){
        elem.second->Hard_stop();
    }
    return axis.size();
}

int Eyrina::G_code_R10(map<char, double> &params, vector<char> &flags){
    
    return 0;
}

void Eyrina::Init_motors (){
    
}
