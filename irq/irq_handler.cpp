#include "irq_handler.hpp"

void IRQ_handler::Notify(){
    if(observer){
        RTOS::Add_thread(string("IRQ"), observer, 2048, RTOS::Priority::High);
    }
    RTOS::IRQ_Signal(this);
}

void IRQ_handler::Register(void (*function_pointer_set)(void)){
    observer = new Invocation_wrapper<void, void, void>(new function<void()>(function_pointer_set));
}
