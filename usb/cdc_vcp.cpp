#include "cdc_vcp.hpp"

#include "globals.hpp"
#include "device/device.hpp"

int USB_CDC::Send(string message){
    transmitting = message;
    uint8_t status = CDC_Transmit_FS((uint8_t *) transmitting.c_str(), transmitting.length());
    return status;
}

int USB_CDC::Receive(uint8_t *Buf, unsigned int Len){
    if (Len == 1) {
        RX_buffer.push_back(Buf[0]);
    } else if (Len > 1) {
        RX_buffer.append(string((char *) Buf, Len));
    } else {
        return 0;
    }
    IRQ->Notify();
    return 0;
}

unsigned short USB_CDC::Add_to_buffer(string &message){
    if (buffer_index_begin == ((buffer_index_end + 1) % buffer_size)) {
        return 0;
    }
    TX_buffer[buffer_index_end] = message;
    buffer_index_end = (buffer_index_end + 1) % buffer_size;
    return abs(buffer_index_begin - buffer_index_end - 1);
}

int USB_CDC::Resend(){
    buffer_index_begin = (buffer_index_begin + 1) % buffer_size;
    if (buffer_index_begin != buffer_index_end) {
        string &message = TX_buffer[buffer_index_begin];
        CDC_Transmit_FS((uint8_t *) TX_buffer.front().c_str(), TX_buffer.front().length());
    }
    return TX_buffer.size();
}

void USB_VCP_RX_Callback(uint8_t *Buf, uint32_t Len){
    device()->mcu->USB_port->Receive(Buf, Len);
}

void USB_VCP_TX_Callback(){
    //device()->mcu->USB_port->Resend();
}
