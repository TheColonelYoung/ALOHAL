/**
 * @file thread.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.11.2019
 */

#pragma once

#include <string>

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "global_includes.hpp"
#include "misc/invocation_wrapper.hpp"

using namespace std;

/**
 * @brief   Redefinition of operator new to use FreeRTOS API
 *
 * @param size      Number of bytes to allocate
 * @return void*    Pointer to allocated memory
 */
void * operator new( size_t size);

/**
 * @brief   Redefinition of operator new to use FreeRTOS API
 *
 * @param size      Number of bytes to allocate
 * @return void*    Pointer to allocated memory
 */
void * operator new[]( size_t size);

/**
 * @brief       Redefinition of operator delete to use FreeRTOS API
 *
 * @param ptr   Pointer to memory which will be freed
 */
void operator delete( void * ptr);

/**
 * @brief       Redefinition of operator delete to use FreeRTOS API
 *
 * @param ptr   Pointer to memory which will be freed
 */
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

/**
 * @brief Wrapper for object and his method which will be start point of thread
 *
 * @tparam class_T Class from which is object
 */
class Thread_seed{

    Invocation_wrapper_base<void, void> *iw;

    bool delete_iw = false;

public:
    Thread_seed() = default;

    Thread_seed(Invocation_wrapper_base<void, void> *iw, bool delete_iw = false) : iw(iw), delete_iw(delete_iw){ };

    ~Thread_seed(){
        if(delete_iw){
            delete iw;
        }
    }

    inline void Run() const {
        iw->Invoke();
    };
};

/**
 * @brief   Create RTOS thread on object and his method.
 *          For stating of thread created lambda function which invocated method on given object.
 *          From object and his method is created Thread_seed which is passed to lambda.
 *          Cannot be called from ISR(IRQ) -> Thread starter must be used.
 *
 * @tparam class_T      Class of object
 * @param name          Name of thread
 * @param object        Object on which is thread started
 * @param method        Method to start as thread entrance point
 * @param priority      RTOS priority of thread
 * @param stack_size    Stack size of thread
 * @param delete_iw     Delete iw after thread is exited?
 * @return osThreadId_t ThreadID
 */
template <typename class_T>
osThreadId_t  Create_thread(string name, class_T *object, void (class_T::*method) (void), Priority priority = Priority::Normal, uint16_t stack_size = 128, bool delete_iw = true){

    auto *iw = new Invocation_wrapper<class_T, void, void>(object, method);

    return Create_thread(name, iw, priority, stack_size, delete_iw);
}

/**
 * @brief   Create RTOS thread from invocation wrapper
 *
 * @param name          Name of thread
 * @param iw            Invocation wrapper which is executed inside thread
 * @param priority      RTOS priority of thread
 * @param stack_size    Stack size of thread
 * @param delete_iw     Delete iw after thread is exited?
 * @return osThreadId_t ThreadID
 */
osThreadId_t  Create_thread(string name, Invocation_wrapper_base<void, void> * iw, Priority priority = Priority::Normal, uint16_t stack_size = 256, bool delete_iw = false);

};
