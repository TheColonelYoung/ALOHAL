#include "voltage_divider.hpp"

Voltage_divider::Voltage_divider(ADC_channel *ADC_IN, double resistor_R1, double resistor_R2) :
    Voltage_sensor(ADC_IN, "Voltage_divider"){
    Register_quantity("voltage", this, &Voltage_divider::Voltage, true);
    if (resistor_R1 != 0 and resistor_R2 != 0) {
        ratio = (resistor_R1 + resistor_R2 / resistor_R2);
    }
}

double Voltage_divider::Voltage(){
    return ratio * ADC_ch->Measure_poll();
}
