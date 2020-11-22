#include "potentiometer.hpp"

Potentiometer::Potentiometer(ADC_channel *ADC_IN, double resistor_R1, double potentiometer_R_VAR_1, double input_voltage) : Voltage_sensor(ADC_IN, "Potentiometer"){
    if (resistor_R1 != 0 and potentiometer_R_VAR_1 != 0) {
        maximal_voltage = (potentiometer_R_VAR_1 / (potentiometer_R_VAR_1 + resistor_R1)) * input_voltage;
    }
}

double Potentiometer::Percentage(){
    double voltage    = Voltage();
    double percentage = 0;

    // Trims are set
    if (trim_lower > 0 and trim_upper > 0) {
        // Trim voltage
        if (voltage > trim_upper) {
            voltage = trim_upper;
        } else if (voltage < trim_lower) {
            voltage = trim_lower;
        }
        // Calculate from trimmed range
        percentage = (voltage - trim_lower) / ((trim_upper - trim_lower) / 100.0);
    } else {
        // Calculate in common way
        percentage = voltage / (maximal_voltage / 100.0);
    }
    return percentage;
}

bool Potentiometer::Trim(double minimal_voltage, double maximal_voltage){
    if (maximal_voltage < minimal_voltage) {
        return false;
    }

    trim_lower = minimal_voltage;
    trim_upper = maximal_voltage;

    return true;
}
