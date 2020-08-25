#include "adc_channel.hpp"

ADC_channel::ADC_channel(AD_C *ADC_IN, int channel):
    ADC_IN(ADC_IN), channel(channel)
{

}

double ADC_channel::Measure_poll(){
    return ADC_IN->Measure_poll(channel);
}
