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

/**
 * @brief   Class encapsulated event for tasker
 *          Event can be executed and contains infformation about:
 *              time to next execution, number of repetitions, period, priority
 *          Execution of performed as Invocation of Invocation wrapper
 *          Events also can be sorted by std::algorithms
 */
class Tasker_event
{
private:
    /**
     * @brief   Name of event
     */
    string name = "";

    /**
     * @brief   Execution target
     */
    Invocation_wrapper_base<void, void> *execution_target = nullptr;

    /**
     * @brief   Number of repetition, -1 for infinity
     */
    int repetition = 0;

    /**
     * @brief   Time between two executions, also functions as delay between first execution
     */
    unsigned long period = 0;

    /**
     * @brief   Remaining time to next execution
     */
    long remaning_time = 0;

    /**
     * @brief   Used when two or more events are synchronized to execute in one moment, higher number = higher priority
     */
    unsigned short priority = 0;

public:
    /**
     * @brief Construct a new Tasker_event object
     *
     * @param name              Name of event
     * @param repetition        Number of repetition, -1 for infinity
     * @param period            Time between two executions, also functions as delay between first execution
     * @param execution_target  Invocation wrapper to execute
     * @param priority          Used when two or more events are synchronized to execute in one moment, higher number = higher priority
     */
    Tasker_event(string name, unsigned int repetition, unsigned long period, Invocation_wrapper_base<void, void> *execution_target, unsigned short priority = 0);

    /**
     * @brief Destroy the Tasker_event object, deletes Invocation wrapper inside
     */
    ~Tasker_event();

    /**
     * @brief   Executes target inside
     *
     * @return true     Event should be removed from que, because is now deprecated, repetition depleted
     * @return false    Event stays alive
     */
    bool Run();

    /**
     * @brief       Comparission operator for events, used for sorting
     */
    bool operator < (const Tasker_event& comp) const
    {
        if (remaning_time == comp.remaning_time){
            return priority > comp.priority;
        }

        return (remaning_time < comp.remaning_time);
    }

    /**
     * @brief   Updates time of event, decrease time to nexte execution
     *
     * @param time  Time from last Update
     * @return long Remaining time to execution
     */
    long Update(long time) { remaning_time -= time; return remaning_time;};

    /**
     * @brief       Remaining time to execution
     *
     * @return long Remaining time to execution
     */
    long Remaining_time() {return remaning_time;};

    /**
     * @brief   Sets repetition to zero and later is deleted
     *          When repetition is set to zero event is removed after appers on top of Tasker scheduler
     */
    void Erase() {repetition = 0;};
};




