#include "pin.hpp"

#include "device/device.hpp"

Pin::Pin(char port, uint8_t pin_number):
    port(port), pin_number(pin_number){
    IRQ = new Pin_IRQ(pin_number, device()->mcu->EXT_IRQ);
}

void Pin::Toggle(){
    HAL_GPIO_TogglePin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin_number);
}

void Pin::Set(bool value){
    HAL_GPIO_WritePin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin_number, (GPIO_PinState) value);
}

bool Pin::Read(){
    return HAL_GPIO_ReadPin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin_number);
}

bool Pin::operator == (const Pin& rhs){
    return ((this->port == rhs.port) && (this->pin_number == rhs.pin_number));
}
