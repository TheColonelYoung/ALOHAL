#include "thread.hpp"

void * operator new( size_t size ){
    return pvPortMalloc( size );
}

void * operator new[]( size_t size ){
    return pvPortMalloc(size);
}

void operator delete( void * ptr ){
    vPortFree ( ptr );
}

void operator delete[]( void * ptr ){
    vPortFree ( ptr );
}

namespace RTOS{
osThreadId_t  Create_thread(string name, Invocation_wrapper_base<void, void> * iw, Priority priority, uint16_t stack_size, bool delete_iw){

    auto * seed = new Thread_seed(iw, delete_iw);

    const osThreadAttr_t defaultTask_attributes = {
        .name     = name.c_str(),
        .stack_size = stack_size,
        .priority = static_cast<osPriority_t>(priority),
    };

    return osThreadNew(
        [](void *seed_void) -> void {
            auto seed = reinterpret_cast<Thread_seed *>(seed_void);
            seed->Run();
            delete seed;
            osThreadExit();
        },
        seed, &defaultTask_attributes);
}

};
