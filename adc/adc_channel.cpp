#include "adc_channel.hpp"

ADC_channel::ADC_channel(AD_C *ADC, int channel):
    ADC(ADC), channel(channel)
{

}

double ADC_channel::Measure_poll(){
    return ADC->Measure_poll(channel);
}
