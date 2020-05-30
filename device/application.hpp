/**
 * @file application.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 16.04.2020
 */

#pragma once

#include <string>
#include <vector>

#include "globals.hpp"
#include "device/device.hpp"

using namespace std;

/**
 * @brief   Interface for any application which can be executed on device
 *          Application can be used event without CLI enabled on device but mostly is supposed to be used with CLI
 */
class Application
{
private:
    /**
     * @brief   Name of appplication used in device structure
     */
    string app_name = "";

public:

    /**
     * @brief   Construct a new Application object
     *          If CLI is enabled created executable in /App folder in device file structure
     *
     * @param name Application name (name of executable)
     */
    Application(string name);

    /**
     * @brief Destroy the Application object
     */
    ~Application() =default;

    /**
     * @brief   Initialization method which is executed at only once at device startup
     *          This method should prepare all external component which is application using
     *
     * @return int  Return code of application, should use errno
     */
    virtual int Init() = 0;

    /**
     * @brief   This method serves as executable for file structure
     *          When application is executed this method is called with possible arguments
     *
     * @param args  Arguments of application
     * @return int  Return code of application, should use errno
     */
    virtual int Run(vector<string> &args) = 0;
    
    /**
     * @brief   This method serves for processing of input from user
     *          When application is running in foreground mode, every line send by user is proccessed by this method
     *
     * @param input     Input line from used for application
     * @return int      Return code defined by application
     */
    virtual int Input_load(string input) = 0;

    /**
     * @brief   Return name of application
     *
     * @return string   Name of application
     */
    inline string Name() const {return app_name;};
};
