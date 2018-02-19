/*
 * Author:	TCY
 * Name:	packet for internet communication
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	5.2.2018
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
#ifndef IP_PACKET_H
#define IP_PACKET_H

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


using namespace std;


// classes
class IP_packet {
public:
    int length;        // Length of data
    string IP_address; // Source address
    int port;          // Source port
    string data;       // Data

    IP_packet() {};
    IP_packet(string IP_address, int port, string data);

    string Print();
};

#endif // ifndef IP_packet_H
