#include "voltage_sensor.hpp"

Voltage_sensor::Voltage_sensor(ADC_channel *ADC, double resistor_R1, double resistor_R2) :
    Sensor("Voltage_sensor"), ADC_ch(ADC), resistor_R1(resistor_R1), resistor_R2(resistor_R2)
{
    Register_quantity("voltage", this, &Voltage_sensor::Voltage, true);
    if (resistor_R1 != 0 and resistor_R2 != 0){
        ratio = (resistor_R1 + resistor_R2 / resistor_R2);
    }
}

double Voltage_sensor::Voltage(){
    return ratio * ADC_ch->Measure_poll();
}
