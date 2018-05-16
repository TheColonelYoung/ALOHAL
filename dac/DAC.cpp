#include "DAC.hpp"

DA_C_channel::DA_C_channel(DA_C *parent, int index) : _parent_convertor(parent), _index(index){ }

void DA_C_channel::Start(){
    if (_index == 0) {
        __HAL_DAC_ENABLE(_parent_convertor->Handler(), DAC_CHANNEL_1);
        HAL_DAC_Start(_parent_convertor->Handler(), DAC_CHANNEL_1);
    } else if (_index == 1) {
        __HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_2);
        HAL_DAC_Start(_parent_convertor->Handler(), DAC_CHANNEL_2);
    }
}

void DA_C_channel::Stop(){
    if (_index == 0) {
        HAL_DAC_Stop(_parent_convertor->Handler(), DAC_CHANNEL_1);
    } else if (_index == 1) {
        HAL_DAC_Stop(_parent_convertor->Handler(), DAC_CHANNEL_2);
    }
}

int DA_C_channel::Set_value(float voltage){
    float supply_voltage = _parent_convertor->supply_voltage;
    if (supply_voltage < 0) {
        return -1;
    }
    int value = (float(4096.0 * voltage) / supply_voltage);
    if (_index == 0) {
        HAL_DAC_SetValue(_parent_convertor->Handler(), DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
    } else if (_index == 1) {
        HAL_DAC_SetValue(_parent_convertor->Handler(), DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
    }
    return value;
}

DA_C::DA_C(DAC_HandleTypeDef *handler, int channel_count) : _handler(handler), _channel_count(channel_count){
    for (int i = 0; i < channel_count; i++) {
        channel.emplace_back(DA_C_channel(this, i));
    }

    supply_voltage = 3.3;
}

DA_C::DA_C(DAC_HandleTypeDef *handler, int channel_count, float supply_voltage) : supply_voltage(supply_voltage){
    DA_C(handler, channel_count);
}

void DA_C::Calibrate(){
    #ifndef ADC_1_EN
    # error Reference voltage must be set or enabled ADC_1
    #else
    supply_voltage = ADC_1.Supply_voltage();
    #endif
}

void DA_C::Test(){
    __HAL_DAC_ENABLE(_handler, DAC_CHANNEL_2);
    HAL_DAC_Start(_handler, DAC_CHANNEL_2);
    int value = (float(4096.0 * 2) / supply_voltage);
    HAL_DAC_SetValue(_handler, DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
}

DAC_HandleTypeDef * DA_C::Handler() {
    return _handler;
}
