/**
 * @file tool.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 12.10.2020
 */

#pragma once

#include <string>

#include "globals.hpp"
#include "device/device.hpp"

using namespace std;

class Tool
{
private:
    string name = "Noname tool";

public:
    Tool(string name);

    /**
     * @brief   Return name of tool
     *
     * @return string   Name of tool
     */
    inline string Name() const {return name;};
};
