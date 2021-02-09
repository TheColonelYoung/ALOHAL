#include "TIM_channel.hpp"

#include "timer/timer.hpp"

TIM_channel::TIM_channel(Timer *parent, uint32_t address)
    : _address(address), _parent_timer(parent){ }


int TIM_channel::PWM_Generate(float percentage){
    if (percentage > 100 or percentage < 0) {
        return -1;
    }

    uint32_t pulse_length = _parent_timer->Autoreload() * (percentage / 100);
    Pulse(pulse_length);
    return static_cast<int>(percentage);
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

    if (mode == Modes::PWM) {
        HAL_TIM_PWM_ConfigChannel(_parent_timer->Handler(), &sConfigOC, _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_ConfigChannel(_parent_timer->Handler(), &sConfigOC, _address);
    }
}

void TIM_channel::Start(){
    if (mode == Modes::PWM) {
        HAL_TIM_PWM_Start(_parent_timer->Handler(), _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_Start(_parent_timer->Handler(), _address);
    }
}

void TIM_channel::Stop(){
    if (mode == Modes::PWM) {
        HAL_TIM_PWM_Stop(_parent_timer->Handler(), _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_Stop(_parent_timer->Handler(), _address);
    }
}

void TIM_channel::Start_DMA(uint32_t *address_from, uint32_t data_size){
    HAL_TIM_OC_Start_DMA(_parent_timer->Handler(), _address, address_from, data_size);
}


void TIM_channel::Pulse(uint32_t pulse_length){
    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.Pulse     = pulse_length;
    sConfigOC.OCMode     = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (mode == Modes::PWM) {
        HAL_TIM_PWM_ConfigChannel(_parent_timer->Handler(), &sConfigOC, _address);
    } else if (mode == Modes::Toggle) {
        HAL_TIM_OC_ConfigChannel(_parent_timer->Handler(), &sConfigOC, _address);
    }
}
