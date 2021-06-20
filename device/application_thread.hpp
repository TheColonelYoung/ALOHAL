/**
 * @file application_thread.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 20.06.2021
 */

#pragma once

#include "device/application.hpp"
#include "misc/invocation_wrapper.hpp"
#include "rtos/thread.hpp"

#include "task.h"

using namespace std;

/**
 * @brief   Variant of application which functionality is executed in thread, otherwise is similar
 *          Reimplements RUn function which start thread
 */
class Application_thread: public Application {
    /**
     * @brief   Priority size with which is thread started
     */
    const RTOS::Priority priority;

    /**
     * @brief   Stack size with which is thread started
     */
    const uint16_t stack_size;

public:
    /**
     * @brief Construct a new Application_thread object
     *
     * @param name          Application name (name of executable)
     * @param stack_size    Stack size of thread in which will be application executed
     * @param priority      Priority of thread in which will be application executed
     */
    Application_thread(string name, RTOS::Priority priority, uint16_t stack_size);

    /**
     * @brief Destroy the Application_thread object
     */
    ~Application_thread() = default;

    /**
     * @brief       Function is called when application is executed
     *              Initialize thread and pass arguments into
     *
     * @param args  Arguments of application, first is name of application
     * @return int  0 if thread was started
     */
    virtual int Run(vector<string> &args) final override;

    /**
     * @brief       Functionality of application
     *              This function muse be implemented by application
     *
     * @param args  Arguments of application, first is name of application
     * @return int  Return code, not relevant when app is oin thread, nowhere to return
     */
    virtual int Thread(vector<string> &args) = 0;

};
