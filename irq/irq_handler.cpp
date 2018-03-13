#include "irq_handler.hpp"
/*
void IRQ_handler::Register(void (*function_pointer_set)(void)){
    IRQ_observer<Pin> obs = IRQ_observer<Pin>();
    obs.Register_f(function_pointer_set);
    observers.push_back(&obs);

}*/

int IRQ_handler::Notify(){

    for(uint i = 0; i < observers.size(); i++) {
        observers[i]->Run();
    }

    return 0;
}
