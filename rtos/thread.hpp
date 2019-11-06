/**
 * @file thread.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.11.2019
 */

#pragma once

#include <string>
#include <unordered_map>

#include "cmsis_os.h"

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

template <typename class_T>
inline osThreadId Create_thread(string name, class_T *object, void (class_T::*method) (void), Priority priority, uint8_t stack_size = 128, uint8_t instances = 1){
    hash<string> hasher;
    int hash_value = hasher(name);
    auto seed      = new Thread_seed<class_T>(object, method);
    osThreadDef(1,
      [](void const *seed_void) -> void {
            auto seed = const_cast<Thread_seed<class_T> *>(reinterpret_cast<const Thread_seed<class_T> *>(seed_void));
            (*(seed->object).*(seed->method))();
            delete seed;
        },
      osPriorityLow, instances, stack_size);
    return osThreadCreate(osThread(1), seed);
}
}
