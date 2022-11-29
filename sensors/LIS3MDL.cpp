#include "LIS3MDL.hpp"

LIS3MDL::LIS3MDL(I2C_master master, unsigned char address):
    I2C_device(master, address),
    Sensor("LIS3MDL")
{
}

uint8_t LIS3MDL::ID(){
    return Register(Registers::WHO_AM_I);
}

array<int16_t, 3> LIS3MDL::Magnetic_field_intensity(){
    array<int16_t, 3> mfi;
    auto register_values = Read(static_cast<uint8_t>(Registers::OUT_X_L),6);
    mfi[0] = (register_values[0] | register_values[1] << 8);
    mfi[1] = (register_values[2] | register_values[3] << 8);
    mfi[2] = (register_values[4] | register_values[5] << 8);
    return mfi;
}

uint8_t LIS3MDL::Register(LIS3MDL::Registers register_name){
        return Read(static_cast<uint8_t>(register_name),1)[0];
}

uint LIS3MDL::Register(LIS3MDL::Registers register_name, uint8_t &value){
        auto data = vector<uint8_t>{value};
        return Write(static_cast<uint8_t>(register_name), data);
}
