/**
 * @file test_application.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 16.04.2020
 */

#pragma once

#include "device/application.hpp"

using namespace std;

class Test_application: public Application
{
public:
    // Inherit constructor and destructor from Application
    using Application::Application;

    virtual int Init() override;

    virtual int Run(vector<string> args) override;
};
