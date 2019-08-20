/**
 * @file g-code_commander.cpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 20.08.2019 B!
 */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "globals.hpp"
#include "construction/motion_axis.hpp"

using namespace std;

class G_code_commander
{
private:
    map<char, Motion_axis*> axis;

public:
    G_code_commander();

    /**
     * @brief Add new axis to list of available axis
     *
     * @param axis_name Identifier of axis
     * @param new_axis  Pointer to new axis (type Motion_axis)
     * @return int      Number of registered axis
     */
    int Add_axis(char axis_name, Motion_axis *new_axis);

    /**
     * @brief Home all axis at once
     *
     * @return int number of axis, which supports homing
     */
    int G28(vector<string> args);

    /**
     * @brief   Move axis by given shift
     *
     * @return int Number of axis, which will be moving
     */
    int G1(vector<string> args);

    /**
     * @brief Perform emergency stop on all axis (Hard_stop)
     *
     * @param args Anythins, is ignored
     * @return int Number of axis
     */
    int E0(vector<string> args);

    /**
     * @brief Perform soft stop at all axis
     *
     * @param args  Anythins, is ignored
     * @return int  Number of axis
     */
    int E1(vector<string> args);


};
