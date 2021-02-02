#include "irq_handler.hpp"

int IRQ_handler::Notify(){
    for (unsigned int i = 0; i < observers.size(); i++) {
        observers[i]->Run();
        //RTOS::Create_thread("IRQ_notify", observers[i], &IRQ_observer_base::Run);
    }

    return 0;
}

void IRQ_handler::Register(void (*function_pointer_set)(void)){
    IRQ_observer<string> *obs = new IRQ_observer<string>();
    obs->Register(function_pointer_set);
    observers.emplace_back(obs);
}
