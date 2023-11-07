#include "device.hpp"

#include "device/component.hpp"
#include "device/application.hpp"

Device* Device::Instance(){
    if (instance == nullptr){
        instance = new Device();
    }
    return instance;
}

Device::Device(){
    Init();
}

void Device::Init(){
    mcu->Init();
}

int Device::Enable_CLI(Serial_line *connection){
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
    fs->Make_directory("/apps");
    fs->Make_directory("/components");
    fs->Make_directory("/mcu");
    fs->Make_directory("/tools");
    mcu->Filesystem_interface_initialization();
    return 0;
}

bool Device::Filesystem_available(){
    return (fs != nullptr);
}

int Device::Register_application(Application *new_application){
    // Test if application with same name already exists
    for( auto &[name, app]: applications){
        if (app->Name() == new_application->Name()){
            return -1;
        }
    }
    applications.insert(make_pair(new_application->Name() ,new_application));
    if(Filesystem_available()){
        fs->Make_executable("/apps/" + new_application->Name(), new_application, &Application::Run);
    }
    return 0;
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
    return original_name + "_#" + to_string(same_name_prefix);
}
