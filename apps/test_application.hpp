/**
 * @file test_application.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 16.04.2020
 */

#pragma once

#include "device/application.hpp"

using namespace std;

/**
 * @brief   Basic demonstrational application
 *          No initialization
 *          On run print arguments and some text
 *
 */
class Test_application: public Application
{
public:
    // Inherit constructor and destructor from Application
    using Application::Application;

    /**
     * @brief Do nothing
     *
     * @return int Always 0
     */
    virtual int Init() final override;

    /**
     * @brief   Print test text on cli if available
     *          Otherwise do nothing
     *
     * @param args  Anything
     * @return int  0 if CLi is available, 1 otherwise
     */
    virtual int Run(vector<string> &args) final override;
};
