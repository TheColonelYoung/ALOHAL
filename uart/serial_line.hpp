/**
 * @file serial_line.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 29.09.2020
 */

#pragma once

#include <vector>
#include <string>

//#include "irq/irq_handler.hpp"

using namespace std;

/**
 * @brief   Common interface and basic functions for serial line interfaces like UART on USB CDC VCP
 */
class Serial_line
{
protected:
    /**
     * @brief String of characters, which was received by serial line
     */
    string RX_buffer = "";

public:

    /**
     * @brief IRQ handler which activates on Receive IRQ
     */
    //IRQ_handler *IRQ = new IRQ_handler();

    /**
     * @brief Construct a new Serial_line object
     */
    Serial_line() = default;

    /**
     * @brief   Sends data via Serial line
     *
     * @param message   String to send
     * @return int      Status code, depends on transmit layer
     */
    virtual int Send(string message) = 0;

    /**
     * @brief Send anything what can be casted to string over serial line
     *
     * @tparam send__type   Type of message which will be converted to string and sended over serial line
     * @param message       Value or text of message
     * @return int          Error code
     */
    template <typename send__type>
    int Send(send__type message){
        return Send(to_string(message));
    }

    /**
     * @brief Transmitt array of strings over serial line char by char
     *
     * @param message   Message to send
     * @return int      Error code
     */
    int Send(const char* message){
        return Send(string(message));
    }

    /**
     * @brief   After IRQ occurs will copy received character from temporal buffer to internal RX buffer
     *
     * @return int  Actual size of RX buffer
     */
    virtual int Receive() = 0;

    /**
     * @brief   Read selected number of characters from internal serial line buffer
     *          After this operation data which are returned are removed from buffer
     *
     * @param length    Number of characters to read
     * @return string   Message from start of buffer with given length
     */
    string Read(int length);

    /**
     * @brief   Read part of buffer which is before delimiter in string
     *          After this operation data which are returned are removed from buffer
     *
     * @param delimiter     Delimiter which borders which part of string is read
     * @return string       Message from start of buffer to delimiter (delimiter is included)
     */
    string Read(string delimiter);

    /**
     * @brief   Return number of characters in RX buffer
     *
     * @return int  Number of characters in RX buffer
     */
    unsigned int Buffer_size() const {return RX_buffer.size();}

    /**
     * @brief   Clear input buffer of serial line
     *
     * @return int  Original size of buffer
     */
    int Clear_buffer();
};
