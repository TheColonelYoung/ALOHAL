#include "device.hpp"

void Device::Init(){
    mcu->Init();
}

int Device::Enable_CLI(UART *connection){
    cli = new CLI();
    if(connection != nullptr){
        cli->Connect(connection);
        cli->Print("CLI is available\r\n");
        cli->Start();
        return 0;
    } else {
        return -1;
    }
}

bool Device::CLI_available(){
    return cli!=nullptr;
}

string Device::Register_component(shared_ptr<Component> new_component){
    components.emplace_back(new_component);
    return New_component_name(new_component->Name());
}

string Device::New_component_name(string original_name){
    int same_name_prefix = count_if(components.begin(), components.end(),
        [original_name](shared_ptr<Component> comp){
            return comp->Name().substr(0, original_name.length()) == original_name;
        });
    return original_name + "_" + to_string(same_name_prefix);
}
