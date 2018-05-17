#include "ADC.hpp"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
    #ifdef ADC_1_EN
    if (hadc->Instance == ADC1) {
        if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC)) {
            ADC_1.Set_value(HAL_ADC_GetValue(hadc));
        }
    }
    #endif
}

AD_C::AD_C(ADC_HandleTypeDef *handler)
    : handler(handler){
    //reference_cal = (*VREFINT_CAL_ADDR);
    HAL_ADCEx_Calibration_Start(handler,0);
}

uint AD_C::Calibration(){
    Set_channel("VREF");
    reference_val  = Measure();
    supply_voltage = (3.3 * ((float) reference_cal / (float) reference_val)) * 1000;
    return supply_voltage;
}

uint16_t AD_C::Value(){
    return ADC_val;
}

int AD_C::Set_resolution(int resolution){
    /*
    switch (resolution) {
        case 12:
            handler->Init.Resolution = ADC_RESOLUTION_12B;
            break;
        case 10:
            handler->Init.Resolution = ADC_RESOLUTION_10B;
            break;
        case 8:
            handler->Init.Resolution = ADC_RESOLUTION_8B;
            break;
        case 6:
            handler->Init.Resolution = ADC_RESOLUTION_6B;
            break;
        default:
            return -1;
    }
    if (HAL_ADC_Init(&hadc) != HAL_OK) {

    }
    resolution = resolution;*/
    return resolution;
}

void AD_C::Set_value(uint16_t value){
    ADC_val = value;
}

void AD_C::Start_IT(){
    /*
    if (handler->Init.EOCSelection != ADC_EOC_SINGLE_CONV) {
        handler->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        HAL_ADC_Init(&hadc);
    }
    */
    HAL_ADC_Start_IT(handler);
}

uint16_t AD_C::Measure(){
    /*
    if (handler->Init.EOCSelection != ADC_EOC_SINGLE_CONV) {
        handler->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        HAL_ADC_Init(&hadc);
    }*/

    HAL_ADC_Start(handler);
    while (1) {
        if (HAL_ADC_PollForConversion(handler, 1000) == HAL_OK) {
            Set_value(HAL_ADC_GetValue(handler));
            break;
        }
    }
    return ADC_val;
}

float AD_C::Voltage_v(){
    return ((float) ADC_val / ((1 << resolution) - 1)) * (supply_voltage / 1000);
}

uint AD_C::Voltage(){
    return ((float) ADC_val / ((1 << resolution) - 1)) * (supply_voltage);
}

uint AD_C::Voltage_u(){
    return ((float) ADC_val / ((1 << resolution) - 1)) * (supply_voltage * 1000);
}

uint AD_C::Supply_voltage(){
    return supply_voltage;
}

float AD_C::Percentage(){
    Measure();
    return ((float) Voltage() / supply_voltage * 100.0);
}

int AD_C::Set_channel(int chan){
    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel      = (uint32_t) chan;
    //sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;

    HAL_ADC_ConfigChannel(handler, &sConfig);
    return 0;
}

int AD_C::Set_channel(string chan){
    ADC_ChannelConfTypeDef chan_config;

    if (chan == "VREF") {
        chan_config.Channel = ADC_CHANNEL_VREFINT;
    } else if (chan == "VBAT") {
        //chan_config.Channel = ADC_CHANNEL_VBAT;
    } else if (chan == "TEMP") {
        chan_config.Channel = ADC_CHANNEL_TEMPSENSOR;
    } else {
        return -1;
    }

    HAL_ADC_ConfigChannel(handler, &chan_config);
    return 0;
}
