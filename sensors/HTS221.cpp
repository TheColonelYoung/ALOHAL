#include "HTS221.hpp"

HTS221::HTS221(I2C_master master, unsigned char address):
    I2C_device(master, address),
    Sensor("HTS221")
{
}

uint8_t HTS221::ID(){
    return Register(Registers::WHO_AM_I);
}

uint8_t HTS221::Register(HTS221::Registers register_name){
        return Read(static_cast<uint8_t>(register_name),1)[0];
}

uint HTS221::Register(HTS221::Registers register_name, uint8_t &value){
        auto data = vector<uint8_t>{value};
        return Write(static_cast<uint8_t>(register_name), data);
}

float HTS221::Temperature(){
    auto register_value_1 = Read(static_cast<uint8_t>(Registers::TEMP_OUT_L),1);
    auto register_value_2 = Read(static_cast<uint8_t>(Registers::TEMP_OUT_H),1);
    int16_t temperature_register = (register_value_1[0] | register_value_2[0] << 8);
    float temperature = (temperature_register * temperature_slope + temperature_zero);
    return temperature;
}

float HTS221::Humidity(){
    auto register_value_1 = Read(static_cast<uint8_t>(Registers::HUM_OUT_L),1);
    auto register_value_2 = Read(static_cast<uint8_t>(Registers::HUM_OUT_H),1);
    int16_t humidity_register = (register_value_1[0] | register_value_2[0] << 8);
    float humidity = (humidity_register * humidity_slope + humidity_zero);
    return humidity;
}

void HTS221::Calibrate(){
    uint8_t h0rH = Register(Registers::CALIB_H0_rH_x2);
    uint8_t h1rH = Register(Registers::CALIB_H1_rH_x2);

    uint16_t t0degC = Register(Registers::CALIB_T0_degC_x8) | ((Register(Registers::CALIB_T1_T0_MSB) & 0x03) << 8);
    uint16_t t1degC = Register(Registers::CALIB_T1_degC_x8) | ((Register(Registers::CALIB_T1_T0_MSB) & 0x0c) << 6);

    int16_t h0t0Out = (Register(Registers::CALIB_H0_T0_OUT_L) | Register(Registers::CALIB_H0_T0_OUT_H) << 8);
    int16_t h1t0Out = (Register(Registers::CALIB_H1_T0_OUT_L) | Register(Registers::CALIB_H1_T0_OUT_H) << 8);

    int16_t t0Out = (Register(Registers::CALIB_T0_OUT_L) | Register(Registers::CALIB_T0_OUT_H) << 8);
    int16_t t1Out = (Register(Registers::CALIB_T1_OUT_L) | Register(Registers::CALIB_T1_OUT_H) << 8);

    humidity_slope = (h1rH - h0rH) / (2.0 * (h1t0Out - h0t0Out));
    humidity_zero = (h0rH / 2.0) - humidity_slope * h0t0Out;

    temperature_slope = (t1degC - t0degC) / (8.0 * (t1Out - t0Out));
    temperature_zero = (t0degC / 8.0) - temperature_slope * t0Out;
}
