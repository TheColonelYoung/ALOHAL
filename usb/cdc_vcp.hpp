/**
 * @file cdc_vcp.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 28.09.2020
 */

/*
Some modifications of HAL USb middleware layer are needed for proper function

Add this to usbd_cdc_if.h (anywhere to begining of file where this will not be rewritten)):
extern void USB_VCP_RX_Callback(uint8_t* Buf, uint32_t Len);
extern void USB_VCP_TX_Callback();

Add this to usbd_cdc_if.c -> CDC_Receive_FS (around line 260) as first line of function:
USB_VCP_RX_Callback(Buf, *Len);

Add this to usbd_cdc_if.c -> CDC_TransmitCplt_FS (around line 310) as first line of function:
USB_VCP_TX_Callback();
*/

#pragma once

#include <string>
#include <vector>

#include "uart/serial_line.hpp"

#include "usbd_cdc_if.h"

using namespace std;

/**
 * @brief   Class for comunication via USB CDC, based on USB middleware layer of STM
 *          Mostly similar to UART
 */
class USB_CDC : public Serial_line
{
private:

    /**
     * @brief Vector of messages which are wainting to be send over USB CDC VCP
     */
    vector<string> TX_buffer;

    /**
     * @brief String which is now transmitted
     */
    string transmitting;

    /**
     * @brief Size of buffer in number of string which can be saved before are send via serial line
     */
    const unsigned short buffer_size = 20;

    /**
     * @brief   Determinates first populated position in buffer
     *          This string is transmitted as first or already in transmittion
     */
    unsigned short buffer_index_begin = 0;

    /**
     * @brief Determinates first empty position in buffer
     */
    unsigned short buffer_index_end = 0;

public:

    /**
     * @brief Construct a new usb cdc object
     */
    USB_CDC() = default;

    /**
     * @brief Transmitt C++ string over USB CDC char by char
     *
     * @param message   Message to send
     * @return int      Error code
     */
    virtual int Send(string message) override final;

    /**
     * @brief   Unused function for receive but must be implemented
     *          Buffer is allocated dynamicly by USb middleware layer and this pointer can change,
     *              mostly during changing of power modes, due to this pointer must be updated in every callback
     *
     * @return int Nothing
     */
    virtual int Receive() override final { return 0; };

    /**
     * @brief       Loads received character into RX buffer and notify all IRQ observers
     *
     * @param Buf   Pointer to USB buffer from which will be character laoded
     * @param Len   Length of text in buffer, only 1 is supported, 2 and more are for non ascii character
     *                   and special symbols
     * @return int  Size of buffer
     */
    virtual int Receive(uint8_t *Buf, unsigned int Len);

    /**
     * @brief   Routine which is called when transmittion is done, will check if buffer contains
     *              another content to send, if yes will send it.
     *
     * @return int  Actual size of transmitt buffer
     */
    int Resend();

private:
    /**
     * @brief Add new message to buffer
     *
     * @param message           Message to save into buffer
     * @return unsigned short   Remaining space in buffer
     */
    unsigned short Add_to_buffer(string &message);
};

/**
 * @brief       Callback function for USB CDC receive complete, must be called from
 *                  CDC_Receive_FS(Src/usbd_cdc_if.c line around 260)
 *
 * @param Buf   Pointer to USB buffer in which are data stored
 * @param Len   Number of characters in buffer
 */
void USB_VCP_RX_Callback(uint8_t *Buf, uint32_t Len);

/**
 * @brief       Callback function fot USB CDC transmit complete, must be called from
 *                  CDC_TransmitCplt_FS(Src/usbd_cdc_if.c line around 310)
 */
void USB_VCP_TX_Callback();
