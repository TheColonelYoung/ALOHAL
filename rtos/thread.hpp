/**
 * @file thread.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.11.2019
 */

#pragma once

#include <string>
#include <unordered_map>

#include "global_includes.hpp"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "misc/invocation_wrapper.hpp"

using namespace std;

void * operator new( size_t size);
void * operator new[]( size_t size);

void operator delete( void * ptr);
void operator delete[]( void * ptr);

namespace RTOS {

enum class Priority {
    Idle        = osPriorityIdle,
    Low         = osPriorityLow,
    BelowNormal = osPriorityBelowNormal,
    Normal      = osPriorityNormal,
    AboveNormal = osPriorityAboveNormal,
    High        = osPriorityHigh,
    Realtime    = osPriorityRealtime,
    Error       = osPriorityError,
};

class RTOS_semaphore {
public:
    osSemaphoreId_t semaphore;

    RTOS_semaphore(uint32_t max_tokens = 1, uint32_t init_tokens = 1){
        semaphore = osSemaphoreNew(max_tokens, init_tokens, NULL);
    }

    void Wait(){
        osSemaphoreAcquire(semaphore, osWaitForever);
    }

    void Fire(){
        osSemaphoreRelease(semaphore);
    }
};

/**
 * @brief Wrapper for object and his method which will be start point of thread
 */
/*
struct Thread_seed_base {
    Thread_seed_base() = default;
    virtual ~Thread_seed_base() = default;
    virtual void Run() const{};
};*/

/**
 * @brief Wrapper for object and his method which will be start point of thread
 *
 * @tparam class_T Class from which is object
 */
struct Thread_seed{

    Invocation_wrapper_base<void, void> *iw;

    bool delete_iw = false;

    osThreadAttr_t config;


    Thread_seed() = default;
    Thread_seed(Invocation_wrapper_base<void, void> *iw, bool delete_iw = false) : iw(iw){ };


    //template <typename>
    void Run() const {
        iw->Invoke();
        if(delete_iw){
            delete iw;
        }
    };
};

/**
 * @brief   Create RTOS thread on object and his method.
 *          For stating of thread created lambda function which invocated method on given object.
 *          From object and his method is created Thread_seed which is passed to lambda.
 *          Cannot be called from ISR(IRQ) -> Thread starter must be used.
 *
 * @tparam class_T Class of object
 * @param name          Name of thread, for identification by hash
 * @param object        Object on which is thread started
 * @param method        Method to start as thread entrance point
 * @param priority      RTOS priority of thread
 * @param stack_size    Stack size of thread
 * @return osThreadId_t ThreadID
 */


template <typename class_T>
inline osThreadId_t  Create_thread(string name, class_T *object, void (class_T::*method) (void), bool delete_iw = false, Priority priority = Priority::Normal, uint8_t stack_size = 128){
    //auto seed      = new Thread_seed<class_T>(object, method);

    auto *iw = new Invocation_wrapper<class_T, void, void>(object, method);

    return Create_thread(name, iw,true, priority, stack_size);
}

inline osThreadId_t  Create_thread(string name, Invocation_wrapper_base<void, void> * iw,  bool delete_iw = false, Priority priority = Priority::Normal, uint8_t stack_size = 128){

    auto * seed = new Thread_seed(iw);

    seed->config = {
        .name       = name.c_str(),
        .stack_size = stack_size,
    };

    return osThreadNew(
        [](void *seed_void) -> void {
            auto seed = reinterpret_cast<Thread_seed *>(seed_void);
            seed->Run();
            delete seed;
            osThreadExit();
        },
        seed, NULL);

}

}
