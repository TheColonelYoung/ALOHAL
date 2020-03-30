#include "TIM_channel.hpp"

#include "timer/timer.hpp"

TIM_channel::TIM_channel(Timer *parent, uint32_t address)
    : _address(address), _parent_timer(parent) { }

int TIM_channel::Set_mode(string mode){
    return 0; // TODO Timer modes
}

int TIM_channel::PWM_Generate(float percentage){
    if (percentage > 100) {
        return -1;
    }

    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.Pulse =  _parent_timer->Handler()->Instance->ARR * (percentage / 100);
    sConfigOC.OCMode     = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(_parent_timer->Handler(), &sConfigOC, _address);

    return static_cast<int>(percentage);
}

void TIM_channel::PWM_Start(){
    HAL_TIM_PWM_Start(_parent_timer->Handler(), _address);
}

void TIM_channel::PWM_Stop(){
    HAL_TIM_PWM_Stop(_parent_timer->Handler(), _address);
}
