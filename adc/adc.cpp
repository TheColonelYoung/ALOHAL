#include "adc.hpp"

#include "device/device.hpp"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
    #ifdef ADC_1_EN
    if (hadc->Instance == ADC1) {
        if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC)) {
            device()->mcu->ADC_1->Set_value(HAL_ADC_GetValue(hadc));
        }
    }
    #endif
}

AD_C::AD_C(ADC_HandleTypeDef *handler, bool vrefint, bool inttemp, bool vbat) :
    handler(handler), has_vref(vrefint), has_itemp(inttemp), has_vbat(vbat){
    Calibration();
}

int AD_C::Calibration(){
    #ifdef MCU_FAMILY_STM32_G0
        HAL_ADCEx_Calibration_Start(handler);
    #else
        HAL_ADCEx_Calibration_Start(handler, 0);
    #endif

    if (has_vref) {
        return Supply_voltage();
    } else {
        return 0;
    }
}

int AD_C::Continuous_measurement(int channel){
    return 0;
}

int AD_C::Continuous_stop(){
    return 0;
}

int AD_C::Measure_IRQ(int channel, unsigned int conversion_count){
    return 0;
}

double AD_C::Measure_poll(int channel){
    Set_channel(channel);
    HAL_ADC_Start(handler);
    if (HAL_ADC_PollForConversion(handler, 10) == HAL_OK) {
        Set_value(HAL_ADC_GetValue(handler));
    } else {
        return -1;
    }

    return (static_cast<float>(ADC_value) / (1<<(static_cast<int>(resolution))) * supply_voltage);
}

double AD_C::Read(){
    return (static_cast<float>(ADC_value) / (1<<(static_cast<int>(resolution))) * supply_voltage);
}

void AD_C::Set_value(uint16_t value){
    ADC_value = value;
}

double AD_C::Supply_voltage(){
    // Test if this ADC have vref channel
    if (!has_vref) {
        return 3.3;
    }

    uint16_t reference_val = Internal_measurement(Internal_channel::VREF);
    if (reference_val == 0) {
        return -1;
    }

    // Calculate internal voltage of core
    uint16_t *reference_cal = INTERNAL_VOLTAGE_REFERENCE_ADDRESS;
    supply_voltage = (3.0f * (*reference_cal) / static_cast<double>(reference_val));

    return supply_voltage;
}

int AD_C::Set_supply_voltage(double supply_voltage){
    int ret = 0;
    if (this->supply_voltage != 0) {
        ret = -1;
    }

    this->supply_voltage = supply_voltage;
    return ret;
}

double AD_C::Core_temperature(){
    if (not has_itemp) {
        return -1;
    }

    uint16_t temperature_val = Internal_measurement(Internal_channel::ITEMP);
    if (temperature_val == 0) {
        return -1;
    }

    // Calculate temperature of core
    uint16_t temperature_cal_1 = *(static_cast<uint16_t *>(INTERNAL_TEMPERATURE_CALIBRATION_1_ADDRESS));
    uint16_t temperature_cal_2 = *(static_cast<uint16_t *>(INTERNAL_TEMPERATURE_CALIBRATION_2_ADDRESS));

    double temperature = (((130.0f - 30.0f) / (temperature_cal_2 - temperature_cal_1)) * (temperature_val * (supply_voltage / 3.0f) - temperature_cal_1) + 30);

    return temperature;
}

double AD_C::Battery_voltage(){
    if (not has_vbat) {
        return -1;
    }

    uint16_t battery_val = Internal_measurement(Internal_channel::VBAT);
    if (battery_val == 0) {
        return -1;
    }

    // Calculate voltage of battery
    double battery_voltage = battery_val * 3.0f / (1<<(static_cast<int>(resolution))) * supply_voltage;

    return battery_voltage;
}

int AD_C::Set_resolution(Resolution resolution){
    handler->Init.Resolution = resolution_map[resolution];
    this->resolution         = resolution;

    if (HAL_ADC_Init(handler) != HAL_OK) {
        return -1;
    }
    return 0;
}

int AD_C::Set_channel_sampling_rate(Sampling_rate sampling_rate){
    channel_config.SamplingTime = sample_rate_map[sampling_rate];

    return Configure_channel();
}

int AD_C::Set_channel(int chan){
    channel_config.Channel = channel_map_common[chan];

    return Configure_channel();
}

int AD_C::Set_channel(Internal_channel internal_channel){
    channel_config.Channel = channel_map_internal[internal_channel];

    return Configure_channel();
}

int AD_C::Configure_channel(){
    if (HAL_ADC_ConfigChannel(handler, &channel_config) != HAL_OK) {
        return -1;
    } else {
        return 0;
    }
}

uint16_t AD_C::Internal_measurement(Internal_channel internal_channel){
    // Save actual setting for conversion
    Resolution old_res = resolution;
    ADC_ChannelConfTypeDef backup_config = channel_config;

    // Set setting for internal voltage conversion
    Set_resolution(Resolution::_12_bit);

    Set_channel(internal_channel);

    /*
    #ifdef ADC_SAMPLETIME_640CYCLES_5
    Set_channel_sampling_rate(Sampling_rate::_640C5);
    #elif defined(ADC_SAMPLETIME_160CYCLES_5)
    Set_channel_sampling_rate(Sampling_rate::_160C5);
    #endif*/

    // Perform conversion and then read value
    HAL_ADC_Start(handler);
    if (HAL_ADC_PollForConversion(handler, 100) != HAL_OK) {
        return 0;
    }
    uint16_t conversion_val = HAL_ADC_GetValue(handler);

    // Set old configuration
    channel_config = backup_config;
    Configure_channel();
    Set_resolution(old_res);

    return conversion_val;
}
