#include "TIM_channel.hpp"

#include "timer/timer.hpp"

TIM_channel::TIM_channel(Timer *parent, uint32_t address)
    : _address(address), parent_timer(parent){ }


int TIM_channel::PWM_Generate(float duty_cycle){
    if(mode != Modes::PWM){
        return -2;
    }

    if (duty_cycle > 100 or duty_cycle < 0) {
        return -1;
    }

    uint32_t pulse_length = parent_timer->Autoreload() * (duty_cycle / 100);
    Capture_compare(pulse_length);
    return 0;
}

void TIM_channel::Mode(Modes mode){
    this->mode = mode;

    uint32_t new_mode = TIM_OCMODE_PWM1;
    switch (mode) {
        case Modes::PWM:
            new_mode = TIM_OCMODE_PWM1;
            break;
        case Modes::Toggle:
            new_mode = TIM_OCMODE_TOGGLE;
            break;
    }

    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode     = new_mode;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (mode == Modes::PWM) {
        HAL_TIM_PWM_ConfigChannel(parent_timer->Handler(), &sConfigOC, _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_ConfigChannel(parent_timer->Handler(), &sConfigOC, _address);
    }
}

void TIM_channel::Start(){
    if (mode == Modes::PWM) {
        HAL_TIM_PWM_Start(parent_timer->Handler(), _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_Start(parent_timer->Handler(), _address);
    }
}

void TIM_channel::Stop(){
    if (mode == Modes::PWM) {
        HAL_TIM_PWM_Stop(parent_timer->Handler(), _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_Stop(parent_timer->Handler(), _address);
    }
}

void TIM_channel::Start_DMA(uint32_t *address_from, uint32_t data_size){
    HAL_TIM_OC_Start_DMA(parent_timer->Handler(), _address, address_from, data_size);
}

uint32_t TIM_channel::Capture_compare(){
    return __HAL_TIM_GET_COMPARE(parent_timer->Handler(), _address);
}

void TIM_channel::Capture_compare(uint32_t capture_compare){
    __HAL_TIM_SET_COMPARE(parent_timer->Handler(), _address, capture_compare);
}
