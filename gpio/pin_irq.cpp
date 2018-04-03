#include "pin_irq.hpp"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    EXT_IRQ.Notify(GPIO_Pin);
}

Pin_IRQ::Pin_IRQ(int pin_number)
    : pin_number(1 << pin_number)
{ }