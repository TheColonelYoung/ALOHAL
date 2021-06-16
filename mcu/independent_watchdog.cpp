#include "independent_watchdog.hpp"

Independent_watchdog::Independent_watchdog(){
    __HAL_DBGMCU_FREEZE_IWDG();
}

void Independent_watchdog::Start(Prescaler prescaler, uint16_t period, uint16_t window){
    handler.Init.Prescaler = static_cast<uint8_t>(prescaler);
    handler.Init.Reload    = period;
    if (window == 0) {
        handler.Init.Window = period;
    } else {
        handler.Init.Window = window;
    }
    HAL_IWDG_Init(&handler);
    __HAL_IWDG_START(&handler);
}

void Independent_watchdog::Reload(){
    HAL_IWDG_Refresh(&handler);
}

uint16_t Independent_watchdog::Calculate_register(float period_ms, Prescaler prescaler, uint16_t frequency, uint8_t counter_size){
    uint16_t divide_factor = 1 << (2 + static_cast<uint8_t>(prescaler));
    return period_ms/1000.0 * frequency / divide_factor;
}
