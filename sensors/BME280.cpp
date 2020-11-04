#include "BME280.hpp"

BME280::BME280(I2C_master master, unsigned char address):
    I2C_device(master, address),
    Sensor("BME280")
{
    // Allocate space in vectors for calibration data
    temperature_calibration.resize(3);
    humidity_calibration.resize(6);
    pressure_calibration.resize(9);

    Register_quantity("Temperature",this, &BME280::Temperature, true);
    Register_quantity("Pressure",this, &BME280::Pressure, true);
    Register_quantity("Humidity",this, &BME280::Humidity, true);

    Load_calibration_data();
}

uint8_t BME280::ID(){
    return Read(static_cast<uint8_t>(Register::ChipID), 1)[0];
}

void BME280::Reset(){
    Write(static_cast<uint8_t>(Register::Reset),{0xb6});
}

bool BME280::Reading_calibration(){
  uint8_t const status_reg = Read(static_cast<uint8_t>(Register::Status), 1)[0];
  return status_reg & (0x01);
}

bool BME280::Measuring(){
  uint8_t const status_reg = Read(static_cast<uint8_t>(Register::Status), 1)[0];
  return status_reg & (0x08);
}

std::uint32_t BME280::Load_ADC(Register ADC_register){
    if (ADC_register == Register::Humidity_data){
        auto ADC_value = Read(static_cast<uint8_t>(ADC_register), 2);
        return (((ADC_value[0] << 8) | ADC_value[1]));
    }   else {
        auto ADC_value = Read(static_cast<uint8_t>(ADC_register), 3);
        return (((ADC_value[0] << 16) | ADC_value[1] << 8) | ADC_value[2]) >> 4;
    }
}

double BME280::Temperature(){
    // Implementation of calculation is from BME280 datasheet
    int32_t var1, var2;

    int32_t adc_T = Load_ADC(Register::Temperature_data);

    var1 =  ((((adc_T >> 3) - ((int32_t)temperature_calibration[0] << 1))) *
                ((int32_t)temperature_calibration[1])) >> 11;

    var2 =  (((((adc_T >> 4) - ((int32_t)temperature_calibration[0])) *
                ((adc_T >> 4) - ((int32_t)temperature_calibration[0]))) >> 12) *
                ((int32_t)temperature_calibration[2])) >> 14;

    t_fine = var1 + var2;

    float T = (t_fine * 5 + 128) >> 8;
    return T / 100;
}

void BME280::Enter_normal_mode(){
    Enter_mode(Modes::Normal);
}

void BME280::Enter_idle_mode(){
    Enter_mode(Modes::Idle);
}

void BME280::Force_measurement(){
    Enter_mode(Modes::Forced);
}

void BME280::Enter_mode(Modes mode){
    uint8_t reg_value = Read(static_cast<uint8_t>(Register::ControlMeas),1)[0];
    reg_value &= 0b11111100;
    reg_value |= static_cast<uint8_t>(mode);
    Write({static_cast<uint8_t>(Register::ControlMeas)},{reg_value});
}

bool BME280::Load_calibration_data(){
    if(calibration_data_loaded){
        return true;
    }

    if(Reading_calibration()){
        Log_line(Log_levels::Error, "Loading of calibration data to registers is still in progress");
        return false;
    }


    // Load temperature calibration: 3x uint16_t
    uint8_t data_address = static_cast<uint8_t>(Register::DIG_T1);
    for (uint8_t data_index = 0; data_index < 3; data_index++){
        auto calibration_value = Read(static_cast<uint8_t>(data_address + data_index*2), 2);
        temperature_calibration[data_index] = (static_cast<std::uint16_t>(calibration_value[1]) << 8) | calibration_value[0];
    }

    // Load pressure calibration: 9x uint16_t
    data_address = static_cast<uint8_t>(Register::DIG_P1);
    for (uint8_t data_index = 0; data_index < 9; data_index++){
        auto calibration_value = Read(static_cast<uint8_t>(data_address + data_index*2), 2);
        pressure_calibration[data_index] = (static_cast<std::uint16_t>(calibration_value[1]) << 8) | calibration_value[0];
    }

    // Load Humidity calibration: 6x uint16_t, but this time is not that simple as above
    humidity_calibration[0] = Read(static_cast<uint8_t>(Register::DIG_H1),1)[0];

    auto calibration_value = Read(static_cast<uint8_t>(Register::DIG_H2), 2);
    humidity_calibration[1] = (calibration_value[1] << 8) | calibration_value[0];

    humidity_calibration[2] = Read(static_cast<uint8_t>(Register::DIG_H3),1)[0];

    calibration_value = Read(static_cast<uint8_t>(Register::DIG_H4), 2);
    humidity_calibration[3] =  (     calibration_value[0] << 4)
                                |   (calibration_value[1] & 0x0f) | 0xf0;

    calibration_value = Read(static_cast<uint8_t>(Register::DIG_H5), 2);
    humidity_calibration[4] =  (     calibration_value[0] & 0x0f)
                                |   (calibration_value[1] << 4 );

    humidity_calibration[5] = Read(static_cast<uint8_t>(Register::DIG_H6),1)[0];

    calibration_data_loaded = true;

    return true;
}
