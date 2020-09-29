#include "cdc_vcp.hpp"

#include "globals.hpp"
#include "device/device.hpp"

int USB_CDC::Send(string message){
    TX_buffer.emplace_back(message);
    uint8_t status = CDC_Transmit_FS((uint8_t *) TX_buffer.front().c_str(), TX_buffer.front().length());
    return status;
}

int USB_CDC::Receive(uint8_t* Buf, unsigned int Len){
    RX_buffer.push_back(Buf[0]);
    IRQ->Notify();
    return 0;
}

int USB_CDC::Resend(){
    TX_buffer.erase(TX_buffer.begin()); // Erase message which transfer is complete
    if (TX_buffer.size() > 0) {         // Send next message in line
        CDC_Transmit_FS((uint8_t *) TX_buffer.front().c_str(), TX_buffer.front().length());
    }
    return TX_buffer.size();
}

void USB_VCP_RX_Callback(uint8_t* Buf, unsigned int Len){
    device()->mcu->USB_port->Receive(Buf, Len);
}

void USB_VCP_TX_Callback(){
    device()->mcu->USB_port->Resend();
}
