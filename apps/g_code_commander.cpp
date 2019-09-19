#include "g_code_commander.hpp"

G_code_commander::G_code_commander(){
    device()->cli->Register_command("G28", "home all axis of device()", this, &G_code_commander::G28);
    device()->cli->Register_command("G1", "Move axis by given distance", this, &G_code_commander::G1);
    device()->cli->Register_command("E0", "Stop all axis immediately", this, &G_code_commander::E0);
    device()->cli->Register_command("E1", "Stop all axis with deceleration", this, &G_code_commander::E1);
}


int G_code_commander::Add_axis(char axis_name, Motion_axis *new_axis){
    if (axis.count(axis_name)){
        device()->cli->Print("Axis already added\r\n");
        return -1;
    }
    axis.insert({axis_name, new_axis});
    return 0;
}

int G_code_commander::G28(vector<string> args){
    device()->mcu->UART_1->Send("G28 - Run:\r\n");
    if(args.size() == 1){
        device()->cli->Print("Home all\r\n");
        for (auto &elem:axis){
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        args.erase(args.begin());
        for(auto &cmd:args){
            if (cmd.length() != 1){
                continue;
            }
            char axis_label = cmd[0];
            if(axis.find(axis_label) != axis.end()){
                axis[axis_label]->GoHome();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int G_code_commander::G1(vector<string> args){
    device()->mcu->UART_1->Send("G1 - Run\r\n");
    args.erase(args.begin());
    int moving_axis = 0;
    // Go through all arguments of command
    for(auto cmd:args){
        char axis_label = cmd[0];
        // If that axis exists
        if ( axis.find(axis_label) != axis.end() ) {
            // If can be parameter converted to string
            if (isdigit(cmd.back())){
                double distance = stod(cmd.substr(1));
                axis[axis_label]->Move(distance);
                moving_axis += 1;
            } else{
                device()->cli->Print("Invalid parameter" + cmd + "\r\n");
            }
        }
    }
    return moving_axis;
}

int G_code_commander::E0(vector<string> args){
    for (auto &elem:axis){
        elem.second->Hard_stop();
    }
    return axis.size();
}

int G_code_commander::E1(vector<string> args){
    for (auto &elem:axis){
        elem.second->Sleep();
    }
    return axis.size();
}
