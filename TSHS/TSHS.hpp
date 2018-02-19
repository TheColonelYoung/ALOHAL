/*
 * Author:	TCY
 * Name:	ESP8266 FOR STM32
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	7.12.2017
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 *
 * --------------------BUGS--------------------
 *
 * ++++++++++++++++++++TODO++++++++++++++++++++
 *
 **************************/
#ifndef TSHS_H
#define TSHS_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif

#include <string>
#include <vector>

#include "ESP/ESP8266.hpp"
#include "gpio/pin.hpp"

class UART;
class ESP8266;

using namespace std;

extern ESP8266 ESP;

// classes
class TSHS {
    ESP8266 *ESP; // communication interface

    string gateway_address = "224.0.0.1";

    string command_buffer;
    vector<IP_packet> received_buffer;

public:
    TSHS(){ };
    TSHS(ESP8266 *ESP);

    int Init();   // Initial setting of TSHS
    void Error(); // This message is called after TSHS responce ERROR
    void Busy();  // This message is called after TSHS responce BUSY
    int Reset();  // Reset TSHS
    int Test();

    string Info(); // Print IP, Gateway, Mask, Broadcast
    string Summary();

    static void Receive_callback();

    void Connect_to_network(string SSID, string password);  //Connect ot Wi-Fi
    int Disconnect();
    int Gateway_discovery();
};
#endif // ifndef TSHS_H
