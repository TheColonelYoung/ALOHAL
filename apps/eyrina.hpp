/**
 * @file eyrina.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 19.04.2020
 */

#pragma once

#include "device/application.hpp"

#include <map>

using namespace std;

class Eyrina : public Application
{
private:

public:
    Eyrina();
    ~Eyrina() = default;

    /**
     * @brief Initialization of all components: motion axis
     *
     * @return int 0 is all is OK, otherwise errno
     */
    virtual int Init() final override { };

    /**
     * @brief   Parse and execute given gcode
     *
     * @param args  G-Code
     * @return int  0 is all is OK, otherwise errno
     */
    virtual int Run(vector<string> args) final override { };

private:

    /**
     * @brief
     *
     * @param gcode
     * @return int
     */
    int Parse(vector<string> gcode);

    int G_code_G1(map<char, double> params, vector<char> flags) {};

    map<string, int (Eyrina::*)(map<char, double>, vector<char> )> g_code_commands = {
        { "G1", &Eyrina::G_code_G1 },
    };
};
