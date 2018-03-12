#include "timer.hpp"

void Timer_IT_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    /*
     * if (htim->Instance == TIM7) { // SM_Handler
     *  SM_DRV8825_Finder();
     * } else if (htim->Instance == TIM2) { // Tasker
     *  Tasker_timer_IT();
     * } else if (htim->Instance == TIM4)  { // PWM1
     *  PWM_Handler(0);
     * } else if (htim->Instance == TIM5)  { // PWM2
     *  PWM_Handler(1);
     * } else if (htim->Instance == TIM6)  { // PWM3
     *  PWM_Handler(2);
     * } else */
    if (htim->Instance == TIM1) { // PWM3
        TIM_1.IRQ_Overflow();
    }
}

int TIM_set_time(TIM_HandleTypeDef *htim, float useconds){
    int prescaler = htim->Instance->PSC + 1;
    float tick    = 1.0 / (TIM_FREQ / prescaler );

    htim->Instance->ARR = useconds / (tick * 1000000);

    return 1;
}

int TIM_get_prescaler(TIM_HandleTypeDef *htim){
    return htim->Instance->PSC;
}

int TIM_get_time(TIM_HandleTypeDef *htim){
    return __HAL_TIM_GET_COUNTER(htim);
}

Timer::Timer(TIM_HandleTypeDef *handler){
    this->handler = handler;
}

Timer::Timer(TIM_HandleTypeDef *handler, int size, int channels){
    this->handler  = handler;
    this->size     = size;

    for(int i = 0; i < channels; i++){
        channels.push_back(PWM_channel());
    }
}

void Timer::IRQ_Overflow(){
    ;
}

void Timer::Time_set(float useconds){
    float tick = 1.0 / (TIM_FREQ / Prescaler_get() );
    Counter_set( (int) (useconds / (tick * 1000000)));
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

uint16_t Timer::Prescaler_get(){
    return handler->Instance->PSC;
}

PWM_channel::PWM_channel(int index){
    this->_index = index;
}
