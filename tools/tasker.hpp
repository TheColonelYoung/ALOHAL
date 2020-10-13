/**
 * @file tasker.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 01.07.2017
 */
#pragma once

#include <string>
#include <list>
#include <algorithm>

#include "globals.hpp"
#include "timer/timer.hpp"
#include "misc/invocation_wrapper.hpp"
#include "tools/tasker_event.hpp"
#include "device/device.hpp"
#include "device/tool.hpp"

using namespace std;

class Tasker: public Tool {
    list<Tasker_event *> events; // vector of tasks

    Timer *timer = nullptr;

    bool running = false; // 0 - tasker stopped, 1 - tasker running

    int free_ID     = 0;  // Id of event original for all run of program
    int overflow    = 0;  // the number of timer overflow
    long delay_time = -1; // time delayed in actual time space

    /**
     * @brief Maximal time which is timer capable to measure with tick every 1us
     */
    unsigned long max_time = 0;


public:
    // constructors
    Tasker(Timer *timer);

    /**
     * @brief   Return number of event registred in Tasker
     *
     * @return unsigned int Number of event registred in Tasker
     */
    unsigned int Event_count() {return events.size();};

    void IRQ();

    template <typename registrator_class>
    void New_event(unsigned long period, int repeats, registrator_class *object, void (registrator_class::*method)(), short priority = 0, string name = "Unnamed"){
        Invocation_wrapper<registrator_class, void, void> *iw = new Invocation_wrapper<registrator_class, void, void>(object, method);
        events.emplace_back(new Tasker_event(name, repeats, period, iw, priority));
        _Sort();

        // If is tasker not running, Start it
        if (events.size() == 1){
            _Start();
        } else {
            // Tasker is running, remaning times needs to be updated and recalculated
            unsigned long time = device()->mcu->TIM_2->Counter_get();
            device()->mcu->TIM_2->Counter_set(0);
            _Update(time);
        }
    }

    string String();

    void Test1(){device()->cli->Print(">test1\r\n");};
    void Test2(){device()->cli->Print(">test2\r\n");};

private:
    /**
     * @brief   Starts timer and enable IRQ
     */
    void _Start();

    /**
     * @brief   Stops timer and disable IRQ
     */
    void _Stop();

    /**
     * @brief   Set timer to specified time
     *
     * @param time  Time to set timer
     */
    void _Set_timer(unsigned long time);

    /**
     * @brief Updates order of event in list
     */
    void _Update(unsigned long time_past = 0);

    void _Sort();
};
