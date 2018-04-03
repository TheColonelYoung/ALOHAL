#include "pin_irq.hpp"

Pin_IRQ::Pin_IRQ(int pin_number)
    : pin_number(1 << pin_number)
{ }
