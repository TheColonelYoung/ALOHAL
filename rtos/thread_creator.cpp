#include "thread_creator.hpp"

#include "thread.hpp"

namespace RTOS{
    Thread_creator Thread_creator_entity;

    Thread_creator::Thread_creator(){
        RTOS::Create_thread("Thread_creator", this, &RTOS::Thread_creator::Loop, Priority::High, 2048, false);
    }

    void Thread_creator::Loop(){
        while(true){
            sem.Acquire();
            unsigned short cycles = initializer_index_top;
            for (size_t i = 0; i < cycles; i++){
                auto record = thread_initializers[initializer_index_top];
                RTOS::Create_thread(record.name, record.iw, record.priority, record.stack_size, false);
                initializer_index_top--;
            }
        }
    }

    void Thread_creator::Add(string name, Invocation_wrapper_base<void, void> *iw, uint16_t stack_size, Priority priority){
        initializer_index_top++;
        thread_initializers[initializer_index_top] = Thread_record(name, iw, stack_size, priority);
        sem.Release();
    }

    void Add_thread(string name, Invocation_wrapper_base<void, void> *iw, uint16_t stack_size, Priority priority){
        Thread_creator_entity.Add(name, iw, stack_size, priority);
    }

}
