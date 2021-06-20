#include "application_thread.hpp"

Application_thread::Application_thread(string name, RTOS::Priority priority, uint16_t stack_size)
    : Application(name), priority(priority), stack_size(stack_size){}

int Application_thread::Run(vector<string> &args){
    auto func = new std::function<void()>(
        [this, &args]() -> void {
        this->Thread(args);
    }
    );
    auto iw = new Invocation_wrapper<void, void, void>(func);
    RTOS::Create_thread(Name(), iw, priority, stack_size, true);
    taskYIELD();
    return 0;
}
