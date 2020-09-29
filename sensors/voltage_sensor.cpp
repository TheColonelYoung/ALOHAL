#include "voltage_sensor.hpp"

Voltage_sensor::Voltage_sensor(ADC_channel *AD_IN, string name) :
    Sensor(name), ADC_ch(AD_IN){
    Register_quantity("voltage", this, &Voltage_sensor::Voltage, true);
}

double Voltage_sensor::Voltage(){
    return ADC_ch->Measure_poll();
}
