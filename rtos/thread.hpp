/**
 * @file thread.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.11.2019
 */

#pragma once

#include <string>
#include <unordered_map>

#include "cmsis_os2.h"
#include "FreeRTOS.h"

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

/**
 * @brief Wrapper for object and his method which will be start point of thread
 *
 * @tparam class_T Class from which is object
 */
template <typename class_T>
struct Thread_seed {
    class_T *object;
    void (class_T::*method) (void);
    Thread_seed(class_T *object, void(class_T::*method)(void)) : object(object), method(method){ }
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
inline osThreadId_t  Create_thread(string name, class_T *object, void (class_T::*method) (void), Priority priority = Priority::Normal, uint8_t stack_size = 128){
    hash<string> hasher;
    int hash_value = hasher(name);
    auto seed      = new Thread_seed<class_T>(object, method);
    return osThreadNew(
        [](void *seed_void) -> void {
            auto seed = reinterpret_cast<Thread_seed<class_T> *>(seed_void);
            (*(seed->object).*(seed->method))();
            delete seed;
        },
        seed, NULL);
}


}
