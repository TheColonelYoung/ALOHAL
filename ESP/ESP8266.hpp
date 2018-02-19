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
#ifndef ESP8266_H
#define ESP8266_H

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

#include "uart/uart.hpp"
#include "ESP/IP_packet.hpp"

#define BOOT_MESSAGE_LENGHT 157

class UART;
class ESP8266;

using namespace std;

extern ESP8266 ESP;
// extern UART
//


// classes
class ESP8266 {
    bool boot             = true;  // Is booting
    bool busy             = false; // Is command performing
    bool connected        = false; // exist connection between ESP and AP
    bool connection_estab = false; // Connection is seized
    bool receiving        = false; // Reception is in progress

    int message_remaining = 0;  // Count of chars, which remain to receive
    int packet_part       = 0;  // Determinates current segment of message : 0-command, 1-length, 2-IP, 3-Port, 4-data
    string packet_temp    = ""; // variable for daving actual part of packet

    UART *ESP_UART;
    vector<string> command_buffer;     // Store command, which will be sended to ESP after previous command will be completed
    vector<IP_packet> received_buffer; // Store messeges sended by ESP, divided by "OK"

    vector<string> data_send; // Data for sending

    void (*rec_callback)(void); // Receive callback function

    string SSID     = "";
    string password = "";

    string ip_address;
    string gateway;
    string mask;
    string broadcast = "224.0.0.1";

public:
    ESP8266(UART_HandleTypeDef *UART_Handler) = delete; // @deprecated
    ESP8266(UART *ESP_UART_set);
    ESP8266(){ };

    int Init();    // Initial setting of ESP8266
    void Error();  // This message is called after ESP responce ERROR
    void Busy();   // This message is called after ESP responce BUSY
    int Reset();   // Reset ESP8266
    int Test();    // Send AT, expects OK
    string Info(); // Print IP, Gateway, Mask, Broadcast

    int Send_command(string AT_command); // Send data via UART add line ending
    int Avaible();                       // Send command waiting in buffer or enable sending
    void Receive();                      // Read data, check if transfer is completed (OK | ready)
    int Process_command();               //   Processs answers of ESP8266

    void Set_SSID(string SSID);                    // Set Wi-Fi SSID
    void Set_password(string password);            // Set Wi-Fi Password
    void Set_rec_callback(void (*function)(void)); // Set Callback function which will be called after receiving message

    int Verbose(bool level);      // Make ESP quiet or verbose
    int Auto_connect(bool state); // Enable or disable autoconnection to AP
    int Remote_IP(bool state);    // Enable or disable showing of remote IP and port in received message

    int Set_mode(int mode_id); // SET EESP8266 mode: 1 - station, 2- SoftAP, 3 - 1+2
    int Connect_to_AP();       // Connect to AP with saved SSID and password
    void IP_info();            // Read info about IP,Gateway,Mask

    int Estab_TCP(string IP, int port); // Estabilish TCP connetion to host
    int Estab_UDP(string IP, int port); // Estabilish UDP connetion to host
    int Packet_receive();               // Parse packet structure
    int Close_connection();

    bool Connection(){ return connected; }

    int Send(string data); // Send data to open connection

    int Parse_IP_info(string answer); // Loads information from IP_info() to object
};
#endif // ifndef ESP8266_H
