#include "timer.hpp"

int TIM_set_time(TIM_HandleTypeDef *htim, float useconds){
    int prescaler = htim->Instance->PSC + 1;
    float tick    = 1000000.0 / (TIM_FREQ / prescaler );

    htim->Instance->ARR = useconds / (tick);

    return htim->Instance->ARR;
}

int TIM_get_prescaler(TIM_HandleTypeDef *htim){
    return htim->Instance->PSC;
}

int TIM_get_time(TIM_HandleTypeDef *htim){
    return __HAL_TIM_GET_COUNTER(htim);
}

Timer::Timer(TIM_HandleTypeDef *handler){
    this->handler   = handler;
    this->frequency = HAL_RCC_GetHCLKFreq();
    this->uticks    = frequency / 1000000.0;
}

Timer::Timer(TIM_HandleTypeDef *handler, int size, int channels)
    : Timer(handler){
    this->size = size;

    for (int i = 1; i <= channels; i++) {
        uint32_t address = 0;
        switch (i) {
            case 1:
                address = 0x0;
                break;
            case 2:
                address = 0x4;
                break;
            case 3:
                address = 0x8;
                break;
            case 4:
                address = 0xC;
                break;
        }

        channel.emplace_back(TIM_channel(this, address));
    }
}
/*
Timer& Timer::operator=(Timer rhs){
    return
}*/

void Timer::Time_set(float useconds){
    if (optimize) {
        Optimize_for(useconds);
    }
    float tick = 1000000.0 / (frequency / (Prescaler() + 1) );
    Counter_set(static_cast<int>((useconds / (tick))));
}

void Timer::Frequency_set(float frequency){
    Time_set(1000000.0 / frequency);
}

bool Timer::Optimize(bool flag){
    optimize = flag;
    return optimize;
}

void Timer::Optimize_for(int time_us){
    handler->Instance->PSC = ((time_us * uticks) / (2 << (size - 1)) - 1) + 1;
}

void Timer::Counter_set(uint32_t new_counter){
    handler->Instance->ARR = new_counter;
}

uint32_t Timer::Counter_get(){
    return handler->Instance->ARR;
}

void Timer::Prescaler_set(uint16_t new_prescaler){
    handler->Instance->PSC = new_prescaler;
}

uint16_t Timer::Prescaler(){
    return handler->Instance->PSC;
}

uint Timer::Input_frequency(){
    return frequency;
}

TIM_HandleTypeDef* Timer::Handler(){
    return handler;
}


void Timer::Start(){
    HAL_TIM_Base_Start(handler);
}

void Timer::Stop(){
    HAL_TIM_Base_Stop(handler);
}

void Timer::Enable_IRQ(){
    HAL_TIM_Base_Start_IT(handler);
}

void Timer::Disable_IRQ(){
    HAL_TIM_Base_Stop_IT(handler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    #ifdef TIM_1_EN
    if (htim->Instance == TIM1) {
        TIM_1.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_2_EN
    if (htim->Instance == TIM2) {
        TIM_2.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_3_EN
    if (htim->Instance == TIM3) {
        TIM_3.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_4_EN
    if (htim->Instance == TIM4) {
        TIM_4.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_5_EN
    if (htim->Instance == TIM5) {
        TIM_5.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_6_EN
    if (htim->Instance == TIM6) {
        TIM_6.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_7_EN
    if (htim->Instance == TIM7) {
        TIM_7.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_8_EN
    if (htim->Instance == TIM8) {
        TIM_8.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_9_EN
    if (htim->Instance == TIM9) {
        TIM_9.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_10_EN
    if (htim->Instance == TIM10) {
        TIM_10.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_11_EN
    if (htim->Instance == TIM11) {
        TIM_11.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_12_EN
    if (htim->Instance == TIM12) {
        TIM_12.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_13_EN
    if (htim->Instance == TIM13) {
        TIM_13.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_14_EN
    if (htim->Instance == TIM14) {
        TIM_14.IRQ.Notify();
        return;
    }
    #endif

    #ifdef TIM_15_EN
    if (htim->Instance == TIM15) {
        TIM_15.IRQ.Notify();
        return;
    }
    #endif
} // HAL_TIM_PeriodElapsedCallback
