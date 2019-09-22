#include "pin_irq.hpp"

#include "device/device.hpp"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    device()->mcu->EXT_IRQ->Notify(GPIO_Pin);
}

Pin_IRQ::Pin_IRQ(int pin_number)
    : pin_number(1 << pin_number)
{ }

IRQ_multi_handler<int> * Pin_IRQ::EXT_IRQ_gettor(){
        return device()->mcu->EXT_IRQ;
}
