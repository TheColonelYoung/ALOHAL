#include "LIS2DW12.hpp"

LIS2DW12::LIS2DW12(I2C_master master, unsigned char address):
    I2C_device(master, address),
    Sensor("LIS2DW12")
{
}

uint8_t LIS2DW12::ID(){
    return Register(Registers::WHO_AM_I);
}

array<int16_t, 3> LIS2DW12::Acceleration(){
    array<int16_t, 3> acceleration;
    auto register_values = Read(static_cast<uint8_t>(Registers::OUT_X_L),6);
    acceleration[0] = (register_values[0] | register_values[1] << 8);
    acceleration[1] = (register_values[2] | register_values[3] << 8);
    acceleration[2] = (register_values[4] | register_values[5] << 8);
    return acceleration;
}


uint8_t LIS2DW12::Register(LIS2DW12::Registers register_name){
        return Read(static_cast<uint8_t>(register_name),1)[0];
}

uint LIS2DW12::Register(LIS2DW12::Registers register_name, uint8_t &value){
        auto data = vector<uint8_t>{value};
        return Write(static_cast<uint8_t>(register_name), data);
}
