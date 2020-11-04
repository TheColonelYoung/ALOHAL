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

    last_known_temperature = var1 + var2;

    int32_t temperature = ((last_known_temperature * 5 + 128) >> 8) - 520;

    return static_cast<double>(temperature) / 100.0f;
}

double BME280::Humidity(){
    // Implementation of calculation is from BME280 datasheet

    int32_t adc_H = Load_ADC(Register::Humidity_data);

    int32_t v_x1_u32r;

    v_x1_u32r = (last_known_temperature - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)humidity_calibration[3]) << 20) -
                    (((int32_t)humidity_calibration[4]) * v_x1_u32r)) +
                    ((int32_t)16384)) >>
                    15) *
                (((((((v_x1_u32r * ((int32_t)humidity_calibration[5])) >> 10) *
                        (((v_x1_u32r * ((int32_t)humidity_calibration[2])) >> 11) +
                        ((int32_t)32768))) >>
                    10) +
                    ((int32_t)2097152)) *
                        ((int32_t)humidity_calibration[1]) +
                    8192) >>
                    14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                                ((int32_t)humidity_calibration[0])) >>
                                4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    float humidity = (v_x1_u32r >> 12);
    return humidity / 1024.0;

}

double BME280::Pressure(){
    // Implementation of calculation is from BME280 datasheet
    int64_t var1, var2, pressure;

    int32_t adc_P = Load_ADC(Register::Pressure_data);

    var1 = ((int64_t)last_known_temperature) - 128000;
    var2 = var1 * var1 * (int64_t)pressure_calibration[5];
    var2 = var2 + ((var1 * (int64_t)pressure_calibration[4]) << 17);
    var2 = var2 + (((int64_t)pressure_calibration[3]) << 35);
    var1 = ((var1 * var1 * (int64_t)pressure_calibration[2]) >> 8) +
            ((var1 * (int64_t)pressure_calibration[1]) << 12);
    var1 =
        (((((int64_t)1) << 47) + var1)) * ((int64_t)pressure_calibration[0]) >> 33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }

    pressure = 1048576 - adc_P;
    pressure = (((pressure << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)pressure_calibration[8]) * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = (((int64_t)pressure_calibration[7]) * pressure) >> 19;

    pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)pressure_calibration[6]) << 4) - 256*100;
    return static_cast<double>(pressure) / 256;
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
