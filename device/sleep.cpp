#include "sleep.hpp"

namespace SLP {

void Sleep(){
    Pin('C', 13).Set(1);
    HAL_SuspendTick();
    //__HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();

}

}
