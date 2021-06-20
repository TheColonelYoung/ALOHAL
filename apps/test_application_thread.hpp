/**
 * @file test_application.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 16.04.2020
 */

#pragma once

#include "device/application_thread.hpp"
#include "rtos/thread.hpp"

using namespace std;

/**
 * @brief   Basic demonstrational application, which executes itself in thread
 *          No initialization, On run print arguments and some text
 */
class Test_application_thread: public Application_thread
{
public:
    // Inherit constructor and destructor from Application_thread
    using Application_thread::Application_thread;

    /**
     * @brief Do nothing
     *
     * @return int Always 0
     */
    virtual int Init() final override;

    /**
     * @brief Print parameters to output
     *
     * @param args
     * @return int
     */
    int Thread(vector<string> &args);

    /**
     * @brief Currently do nothing
     *
     * @param input     Ignored
     * @return int      Only 0
     */
    virtual int Input_load(string input) final override;
};
