#include "LED_driver.hpp"

LED_Driver::LED_Driver(uint maximal_current, uint allowed_current) :
    LED_Driver("Unknown_LED_driver", maximal_current, allowed_current)
{ }

LED_Driver::LED_Driver(string name, uint maximal_current, uint allowed_current) :
    Component(name),
    maximal_current(maximal_current),
    allowed_current(allowed_current)
{ }
