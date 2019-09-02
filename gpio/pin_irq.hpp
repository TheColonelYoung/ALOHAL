#ifndef PIN_IRQ_HPP
#define PIN_IRQ_HPP

// ------------------LIBRARY_OPTIONS----------------
// ------------------LIBRARY_OPTIONS----------------
#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif

#include "irq/irq_multi_handler.hpp"
extern IRQ_multi_handler<int> EXT_IRQ;

class Pin_IRQ {

public:
    enum Trigger{
        Disabled,
        Rising_edge,
        Falling_edge,
        On_change
    };

    int pin_number;

    Pin_IRQ() =default;
    Pin_IRQ(int pin_number);

    template <typename registrator_class>
    void Register(registrator_class& object, void (registrator_class::*method_pointer_set)()){
        EXT_IRQ.Register(pin_number, object, method_pointer_set);
    }

    void Register(void (*function_pointer_set)(void)){
        EXT_IRQ.Register(pin_number,function_pointer_set);
    }

};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


#endif // ifndef PIN_IRQ_HPP
