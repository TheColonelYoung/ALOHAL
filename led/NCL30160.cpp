#include "NCL30160.hpp"

NCL30160::NCL30160(TIM_channel *source_channel, uint maximal_current, uint allowed_current) :
    LED_Driver("NCL30160", maximal_current, allowed_current),
    source_channel(source_channel)
{ }

uint NCL30160::Current(uint current){
    if (current > 0) {
        double PWM_duty = static_cast<double>(current) / maximal_current;
        source_channel->PWM_Generate(PWM_duty);
        source_channel->PWM_Start();
    } else if (current == 0) {
        source_channel->PWM_Stop();
    }
    return actual_current;
}
