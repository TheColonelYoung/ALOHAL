#ifndef IRQ_HANDLER_HPP
#define IRQ_HANDLER_HPP

// ------------------LIBRARY_INCLUDES--------------

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

using namespace std;

#include <vector>
#include <string>

#include "irq_observer.hpp"

class IRQ_handler {
public:
    vector<IRQ_observer_base *> observers;

    IRQ_handler() = default;

    template <class registrator_class>
    void Register(registrator_class& object, void (registrator_class::*method_pointer_set)()){
        IRQ_observer<registrator_class> *obs = new IRQ_observer<registrator_class>();
        obs->Register(object, method_pointer_set);
        observers.emplace_back(obs);
    }

    template <class registrator_class>
    void Register(registrator_class* object, void (registrator_class::*method_pointer_set)()){
        IRQ_observer<registrator_class> *obs = new IRQ_observer<registrator_class>();
        obs->Register(object, method_pointer_set);
        observers.emplace_back(obs);
    }

    void Register(void (*function_pointer_set)(void));

    template <class unregistrator_class>
    void Unregister(unregistrator_class& object){
        for (unsigned int i = 0; i < observers.size(); i++) {
            if (&object == observers[i]->Get_origin_object()) {
                delete(observers[i]);
                observers.erase(observers.begin() + i);
            }
        }
    }

    int Notify();
};

#endif // ifndef IRQ_HANDLER_HPP
