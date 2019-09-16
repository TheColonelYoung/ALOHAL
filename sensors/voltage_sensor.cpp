#include "voltage_sensor.hpp"

Voltage_sensor::Voltage_sensor(ADC_channel *ADC, double resistor_R1, double resistor_R2) :
    Sensor("Voltage_sensor"), ADC(ADC), resistor_R1(resistor_R1), resistor_R2(resistor_R2){
    Register_quantity("voltage", this, &Voltage_sensor::Voltage, true);
}

double Voltage_sensor::Voltage(){
    return 5.0065164f;
}
