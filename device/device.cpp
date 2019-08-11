/**
 * @file device.cpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 11.07.2019
 */


#include "device.hpp"

void Device::Init(){
    mcu->Init();
    cli->Connect(mcu->UART_2);
    mcu->UART_2.Send("Initialization done\r\n");
    cli->Start();
}

