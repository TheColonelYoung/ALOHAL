#include "timer.hpp"

#include "device/device.hpp"

Timer::Timer(TIM_HandleTypeDef *handler){
    this->handler   = handler;
    this->frequency = HAL_RCC_GetHCLKFreq();
    this->uticks    = frequency / 1000000.0;

    // Clear timer IRQ flag after init
    __HAL_TIM_CLEAR_FLAG(handler, TIM_SR_UIF);
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

void Timer::Time_set(float useconds, bool optimize){
    if (optimize or this->optimize) {
        Optimize_for(useconds);
    }
    float tick = 1000000.0 / (frequency / (Prescaler() + 1) );
    handler->Instance->ARR = static_cast<int>((useconds / (tick)));
}

void Timer::Frequency_set(float frequency){
    Time_set(1000000.0 / frequency);
}

void Timer::Optimize_for(int time_us){
    handler->Instance->PSC =  ((unsigned long long)time_us * uticks) / ((unsigned long long)1 << size);
}

void Timer::Start(){
    if(mode == Modes::Timer){
        HAL_TIM_Base_Start(handler);
    } else if (mode == Modes::Timer_IRQ){
        HAL_TIM_Base_Start_IT(handler);
    }
}

void Timer::Stop(){
    if(mode == Modes::Timer){
        HAL_TIM_Base_Stop(handler);
    } else if (mode == Modes::Timer_IRQ){
        HAL_TIM_Base_Stop_IT(handler);
    }
}

#ifdef TIMER_USED_AS_TIMEBASE
void ALOHAL_Timer_IRQ_Callback(TIM_HandleTypeDef *htim){
#else
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
#endif
    #ifdef TIM_1_EN
    if (htim->Instance == TIM1) {
        device()->mcu->TIM_1->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_2_EN
    if (htim->Instance == TIM2) {
        device()->mcu->TIM_2->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_3_EN
    if (htim->Instance == TIM3) {
        device()->mcu->TIM_3->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_4_EN
    if (htim->Instance == TIM4) {
        device()->mcu->TIM_4->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_5_EN
    if (htim->Instance == TIM5) {
        device()->mcu->TIM_5->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_6_EN
    if (htim->Instance == TIM6) {
        device()->mcu->TIM_6->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_7_EN
    if (htim->Instance == TIM7) {
        device()->mcu->TIM_7->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_8_EN
    if (htim->Instance == TIM8) {
        device()->mcu->TIM_8->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_9_EN
    if (htim->Instance == TIM9) {
        device()->mcu->TIM_9->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_10_EN
    if (htim->Instance == TIM10) {
        device()->mcu->TIM_10->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_11_EN
    if (htim->Instance == TIM11) {
        device()->mcu->TIM_11->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_12_EN
    if (htim->Instance == TIM12) {
        device()->mcu->TIM_12->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_13_EN
    if (htim->Instance == TIM13) {
        device()->mcu->TIM_13->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_14_EN
    if (htim->Instance == TIM14) {
        device()->mcu->TIM_14->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_15_EN
    if (htim->Instance == TIM15) {
        device()->mcu->TIM_15->IRQ->Notify();
        return;
    }
    #endif

    #ifdef TIM_16_EN
    if (htim->Instance == TIM16) {
        device()->mcu->TIM_16->IRQ->Notify();
        return;
    }
    #endif
} // HAL_TIM_PeriodElapsedCallback
