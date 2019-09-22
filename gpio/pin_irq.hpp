/**
 * @file pin_irq.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 */

#pragma once

#include "global_includes.hpp"
#include "globals.hpp"
#include "irq/irq_multi_handler.hpp"

/**
 * @brief Respresent IRQ subsystem of interrupt capable pin
 *        Every pin, which is IRQ capable contains this class or class derivated from this class
 */
class Pin_IRQ {
public:

    /**
     * @brief Available states to which can be any IRQ capable pin setup
     */
    enum class Trigger {
        Disabled,
        Rising_edge,
        Falling_edge,
        On_change
    };

    int pin_number;

    Pin_IRQ() = default;

    /**
     * @brief Construct a new Pin_IRQ object
     *
     * @param pin_number Interrupt channel of pin (pin number)
     */
    Pin_IRQ(int pin_number);

    /**
     * @brief   Register object and his method as callback, when external interrupt occurs
     *              on selected IRQ channel
     *
     * @tparam class_T  Class from which is object, which perform registration
     * @param object    Pointer to object performing registration
     * @param method_pointer_set    pointer to method of class from which is object which will be invocated
     */
    template <typename class_T>
    void Register(class_T *object, void (class_T::*method_pointer_set)()){
        EXT_IRQ_gettor()->Register(pin_number, object, method_pointer_set);
    }

    /**
     * @brief   Register a function as callback for external interrupt
     *
     * @param function_pointer_set Pointer to method, which will be called
     */
    void Register(void (*function_pointer_set)(void)){
        EXT_IRQ_gettor()->Register(pin_number, function_pointer_set);
    }

private:

    /**
     * @brief   This method is only wrapper around pointer to multi handler which serves for
     *              external interrupts, because pointer cannot abe obtained directly from singleton
     *          This problem is due to dependency loop, so singleton or wrapper around singleton
     *              cannot be called from .hpp file, so cannot be used in templates
     *
     * @return IRQ_multi_handler<int>* Pointer to multihandler which serves external interrupts
     */
    IRQ_multi_handler<int> * EXT_IRQ_gettor();
};

/**
 * @brief Callback which is called by HAL when IRQ event occurs at some
 *           IRQ capable and configured pin
 *
 * @param GPIO_Pin Number of IRQ line to which is Pin assigned
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
