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
#include <functional>

#include "globals.hpp"
#include "timer/timer.hpp"
#include "misc/invocation_wrapper.hpp"
#include "tools/tasker_event.hpp"
#include "device/device.hpp"
#include "device/tool.hpp"

using namespace std;

/**
 * @brief   Tasker serves for execution of low priority events with low frequency (mostly below 10 Hz)
 *          Precision of Tasker is not good (up to miliseconds), because can contain more event planned to same time
 *          Example usage of Tasker is to execute LED toggle every 500ms
 *          Tasker can also be used to delay some event (creating event with one repetition)
 *          The is no mechanism for paralel safety between events and normal program
 */
class Tasker: public Tool {
    /**
     * @brief Singleton instance
     */
    inline static Tasker* instance = nullptr;

    /**
     * @brief Sorted event which will be executed
     */
    list<Tasker_event *> events;

    /**
     * @brief Timer used by Tasker for event timing
     */
    Timer *timer = nullptr;

    /**
     * @brief Status flag of tasker, 0 - tasker stopped, 1 - tasker running
     */
    bool running = false;

    /**
     * @brief Maximal time which is timer capable to measure with tick every 1us
     */
    unsigned long max_time = 0;

private:
    /**
     * @brief       Construct a new Tasker object
     *
     * @param timer Timer used by Tasker for event timing
     */
    Tasker(Timer *timer);

public:
    /**
     * @brief   Singleton instance with class initialization
     *          Must be callled before version of Instance without initialization
     *
     * @return Tasker* One and only instance of Tasker
     */
    static Tasker* Instance(Timer *timer);

    /**
     * @brief   Singleton instance without initialization
     */
    static Tasker* Instance();

    /**
     * @brief   Adds new Event to tasker queue, creates new Invocation wrapper inside
     *
     * @tparam registrator_class    Class of object from which to create and Invocation wrapper
     * @param period    Time between two executions, also functions as delay between first execution
     * @param repeats   Number of repetition, -1 for infinity
     * @param object    Pointer to object
     * @param method    Pointer to method of class, from which object is
     * @param priority  Used when two or more events are synchronized, higher number = higher priority
     * @param name      Name of event
     */
    template <typename registrator_class>
    Tasker_event * New_event(unsigned long period, int repeats, registrator_class *object, void (registrator_class::*method)(), short priority = 0, string name = ""){
        Invocation_wrapper<registrator_class, void, void> *iw = new Invocation_wrapper<registrator_class, void, void>(object, method);
        return New_event(period, repeats, iw, priority, name);
    }

    /**
     * @brief   Adds new Event to tasker queue
     *
     * @tparam registrator_class    Class of object from which to create and Invocation wrapper
     * @param period    Time between two executions, also functions as delay between first execution
     * @param repeats   Number of repetition, -1 for infinity
     * @param iw        Invocation_wrapper containing (object and method) or function
     * @param priority  Used when two or more events are synchronized, higher number = higher priority
     * @param name      Name of event
     */
    template <typename registrator_class>
    Tasker_event * New_event(unsigned long period, int repeats, Invocation_wrapper<registrator_class, void, void> *iw, short priority = 0, string name = ""){
        auto new_event = new Tasker_event(name, repeats, period, iw, priority);
        events.emplace_back(new_event);
        _Sort();

        // If is tasker not running, Start it
        if (events.size() == 1){
            _Start();
        } else {
            // Tasker is running, remaning times needs to be updated and recalculated
            unsigned long time = timer->Counter();
            timer->Counter(0);
            _Update(time);
        }
        return new_event;
    }

    /**
     * @brief   Poll defined function (Invocation wrapper) until condition is fulfilled after that executes callback
     *          Polled function must return boolean or boolean castable value
     *
     * @tparam condition_result     Boolean to which is return value of polled function compared
     * @tparam polled_class         Class with which can be polled invocation wrapper templated
     * @tparam callback_class       Class with which can be callback invocation wrapper templated
     * @param polled_object     Invocation wrapper which will be polled
     * @param period            Period in us in which will be function polled
     * @param callback          Callback (Invocation wrapper) which will be executed after fulfilling the condition
     */
    template <bool condition_result, typename polled_class, typename callback_class>
    void Poll_until(Invocation_wrapper<polled_class, bool, void> *polled_object, unsigned long period, Invocation_wrapper<callback_class, void, void> *callback){
        // Pointer to pointer in which will be pointing to new event
        Tasker_event ** event_pointer = new Tasker_event *;
        // Create lambda function which will be polling, comparing and executing callback
        // Lambda captures pointer to pointer to Tasker event in which will be this lambda saved
        function<void()> *event_function = new function<void()>(
            [polled_object, callback, event_pointer]() -> void{
                // Pooling target
                if(polled_object->Invoke() == condition_result){
                    // Executing callback
                    callback->Invoke();
                    if(*event_pointer){
                        // Erasing tasker event which is executing this lambda, event is created below
                        (*event_pointer)->Erase();
                        // Delete pointer to pointer to event of this lambda
                        delete (event_pointer);
                    }
                }
            }
        );
        // Create wrapper containing lambda
        Invocation_wrapper<void, void, void> *iw  = new Invocation_wrapper<void, void, void>(event_function);
        // Create new event and list it into Tasker
        // Also pointer to this event is saved into space where lambda has pointer
        *event_pointer = New_event(period, -1, iw, 0, "Poller");
    }

    /**
     * @brief   Return number of event registred in Tasker
     *
     * @return unsigned int Number of event registred in Tasker
     */
    unsigned int Event_count() {return events.size();};

    /**
     * @brief   IRQ callback called by Timer
     */
    void IRQ();

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

    /**
     * @brief   Sort event in queue, based on remaining_time and priority
     */
    void _Sort();
};
