#include "LED_driver.hpp"

LED_Driver::LED_Driver(uint maximal_current, uint allowed_current) :
    LED_Driver("Unknown_LED_driver", maximal_current, allowed_current)
{ }

LED_Driver::LED_Driver(string name, uint maximal_current, uint allowed_current) :
    Component(name),
    maximal_current(maximal_current),
    allowed_current(allowed_current)
{
    if (allowed_current == 0) {
        allowed_current = maximal_current;
    }
}

double LED_Driver::Power(float power){
    if ((power > 100) | (power < 0)){
        return -1;
    }
    Current((power/100)*maximal_current);
    return Power();
}

double LED_Driver::Power() const{
    return 100 * (static_cast<double>(actual_current) / maximal_current);
}

double LED_Driver::Power_limit() const{
    return 100 * (static_cast<double>(allowed_current) / maximal_current);
}
