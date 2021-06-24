/**
 * @file utils.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 22.06.2021
 */

#pragma once

#include "misc/invocation_wrapper.hpp"

#include "cmsis_os2.h"

using namespace std;

namespace RTOS {
template <bool condition_result, typename polled_class, typename callback_class>
void Poll_until(Invocation_wrapper<polled_class, bool, void> *polled_object, unsigned long period_ms, Invocation_wrapper<callback_class, void, void> * callback = nullptr){
    while(true){
        if(polled_object->Invoke() == condition_result){
            // Executing callback
            if(callback){
                callback->Invoke();
            }
        }else{
            // Wait for another window
            osDelay(period_ms);
        }
    }
}

template <bool condition_result, typename polled_class>
void Poll_until(Invocation_wrapper<polled_class, bool, void> polled_object, unsigned long period_ms){
    while(true){
        if(polled_object.Invoke() == condition_result){
            return;
        }else{
            // Wait for another window
            osDelay(period_ms);
        }
    }
}

};
