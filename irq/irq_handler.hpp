/**
 * @file irq_handler.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 14.06.2021
 */

#pragma once

using namespace std;

#include <string>
#include <functional>

#include "irq_observer.hpp"
#include "misc/invocation_wrapper.hpp"

/**
 * @brief   Generic class for handling of interputs
 *          When MCU IRQ occurs notification is send to this class via Notify
 *          Callback in form of Invocation wrapper can be registred.
 *          When IRQ occurs callback is started in separete thread.
 *          Another option is to wait for Event Flag, this event flag is raised when IRQ occurs.
 *          Signaling and waiting for event is done via RTOS::IRQ_Signal and RTOS::IRQ_Wait
 *          Callback thread is better if IRQ occurs not very often.
 */
class IRQ_handler {
    /**
     * @brief   Callback which is passed to new thread when IRQ occurs
     */
    Invocation_wrapper_base<void, void> *observer = nullptr;

    /**
     * @brief  Unique ID of IRq handler, used by event signaling
     */
    const uint16_t id;

    /**
     * @brief Shared counter for ID initialization
     */
    inline static uint16_t irq_id_counter;

public:
    /**
     * @brief Construct a new irq handler object
     */
    IRQ_handler(): id(irq_id_counter){
        irq_id_counter++;
    }

    /**
     * @brief Destroy the irq handler object, unregister observer
     */
    ~IRQ_handler(){
        Unregister();
    }

    /**
     * @brief Register new observer
     *
     * @tparam registrator_class    Class of registred object
     * @param object                Reference to registered object
     * @param method_pointer_set    pointer to method of registred class
     */
    template <class registrator_class>
    void Register(registrator_class& object, void (registrator_class::*method_pointer_set)()){
        observer = new Invocation_wrapper<registrator_class, void, void>(&object, method_pointer_set);
    }

    /**
     * @brief Register new observer
     *
     * @tparam registrator_class    Class of registred object
     * @param object                Pointer to registered object
     * @param method_pointer_set    pointer to method of registred class
     */
    template <class registrator_class>
    void Register(registrator_class* object, void (registrator_class::*method_pointer_set)()){
        observer = new Invocation_wrapper<registrator_class, void, void>(object, method_pointer_set);
    }

        /**
     * @brief Register new observer
     *
     * @tparam registrator_class    Class of registred object
     * @param iw                    Invocation wrapper containing callback
     */
    template <class registrator_class>
    void Register(Invocation_wrapper<registrator_class, void, void>* iw){
        observer = iw;
    }

    /**
     * @brief Register function as observer
     *
     * @param function_pointer_set
     */
    void Register(void (*function_pointer_set)(void));

    /**
     * @brief Removes observer
     */
    void Unregister(){
        delete observer;
        observer = nullptr;
    }

    /**
     * @brief   Returns ID of IRQ handler
     *
     * @return const uint16_t   ID of IRQ handler
     */
    const uint16_t ID(){return id;};

    /**
     * @brief Perform notification of IRQ handler, called from IRQ routine
     *
     * @return int
     */
    void Notify();
};

#include "rtos/irq_director.hpp"
