#include "device.hpp"

void Device::Init(){
    mcu->Init();
}

int Device::Enable_CLI(UART *connection){
    cli = new CLI();
    if(connection != nullptr){
        cli->Connect(connection);
        cli->Print("CLI is available\r\n");
        HAL_Delay(1);
        cli->Start();
        return 0;
    } else {
        return -1;
    }
}

bool Device::CLI_available(){
    return cli!=nullptr;
}

int Device::Enable_Filesystem(){
    if (!CLI_available()){
        return -1;
    }
    fs = new Filesystem(cli);
    fs->Make_directory("/components");
    return 0;
}

bool Device::Filesystem_available(){
    return (fs != nullptr);
}

string Device::Register_component(Component* new_component){
    components.emplace_back(new_component);
    string new_name = New_component_name(new_component->Name());
    if(Filesystem_available()){
        fs->Make_directory("/components/" + new_name);
    }
    return new_name;
}

string Device::New_component_name(string original_name){
    int same_name_prefix = count_if(components.begin(), components.end(),
        [original_name](Component* comp){
            return comp->Name().substr(0, original_name.length()) == original_name;
        });
    return original_name + "_" + to_string(same_name_prefix);
}
