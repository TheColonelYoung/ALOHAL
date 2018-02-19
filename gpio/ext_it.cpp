#include "ext_it.hpp"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_14){
        Pin('C',13).Set(1);
    }
}
