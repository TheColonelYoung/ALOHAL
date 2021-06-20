#include "irq_handler.hpp"

#include "rtos/irq_director.hpp"
#include "rtos/thread_creator.hpp"

void IRQ_handler::Notify(){
    if(observer){
        RTOS::Add_thread("IRQ", observer);
    }
    RTOS::IRQ_Signal(this);
}

void IRQ_handler::Register(void (*function_pointer_set)(void)){
    observer = new Invocation_wrapper<void, void, void>(new function<void()>(function_pointer_set));
}
