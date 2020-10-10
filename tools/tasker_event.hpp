/**
 * @file event.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.10.2020
 */

#pragma once

#include <string>

#include "misc/invocation_wrapper.hpp"

using namespace std;

class Tasker_event
{
private:
    string name = "None";

    Invocation_wrapper_base<void, void> *execution_target = nullptr;

    int repetition = 0;

    unsigned long period = 0;

    long remaning_time = 0;

    unsigned short priority = 0;

public:
    Tasker_event(string name, unsigned int repetition, unsigned long period, Invocation_wrapper_base<void, void> *execution_target, unsigned short priority = 0);

    ~Tasker_event();

    bool Run();

    bool operator < (const Tasker_event& comp) const
    {
        if (remaning_time == comp.remaning_time){
            return priority > comp.priority;
        }

        return (remaning_time < comp.remaning_time);
    }

    long Update(long time) { remaning_time -= time; return remaning_time;};

    long Remaining_time() {return remaning_time;};

    string String();
};




