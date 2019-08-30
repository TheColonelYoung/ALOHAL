/**
 * @file device.cpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */


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
