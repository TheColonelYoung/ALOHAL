#ifndef IRQ_OBSERVER_HPP
#define IRQ_OBSERVER_HPP

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

#include <functional>

using namespace std;

class IRQ_observer_base{
public:
    IRQ_observer_base()= default;
    virtual ~IRQ_observer_base()= default;
    virtual void Register(){}
    virtual void Run(){}
    virtual void *Get_origin_object() = 0;
};

template <typename observer_class>
class IRQ_observer: public IRQ_observer_base{
    void * pointer_to_origin;
    observer_class object;
    void (observer_class::*method_pointer)();
    function<void(observer_class&)> callback;

public:
    IRQ_observer()= default;

    void Register(observer_class& object_set, void (observer_class::*method_pointer_set)()){
        pointer_to_origin = &object_set;
        object = object_set;
        method_pointer = method_pointer_set;
    }

    void Run() override{
        (object.*method_pointer)();
    }

    void *Get_origin_object() override{
        return pointer_to_origin;
    }
};


#endif // ifndef IRQ_OBSERVER_HPP
