#include "NCL30160.hpp"

NCL30160::NCL30160(TIM_channel *source_channel, uint maximal_current, uint allowed_current) :
    LED_Driver("NCL30160", maximal_current, allowed_current),
    source_channel(source_channel)
{
    source_channel->Mode(TIM_channel::Modes::PWM);
}

uint NCL30160::Set_output_current(uint current){
    if (current > 0) {
        double PWM_duty = static_cast<double>(current) / maximal_current;
        source_channel->PWM_Generate(PWM_duty * 100);
        source_channel->Start();
    } else if (current == 0) {
        source_channel->Stop();
    }
    return current;
}
