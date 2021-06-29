#include "cdc_vcp.hpp"

#include "globals.hpp"
#include "device/device.hpp"

USB_CDC::USB_CDC(){
    RX_buffer.reserve(APP_RX_DATA_SIZE);
}

int USB_CDC::Send(string message){
    if (message.length() == 0) {
        return 0;
    }

    transmitting = message;
    return CDC_Transmit_FS((uint8_t *) transmitting.c_str(), transmitting.length());
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

int USB_CDC::Resend(){
    return 0;
}

void USB_VCP_RX_Callback(uint8_t *Buf, uint32_t Len){
    device()->mcu->USB_port->Receive(Buf, Len);
}

void USB_VCP_TX_Callback(){
    // Unused due to issue with HAL
    //device()->mcu->USB_port->Resend();
}
