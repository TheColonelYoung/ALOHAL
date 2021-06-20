/**
 * @file thread_creator.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 14.06.2021
 */

#pragma once

#include <deque>
#include <string>

#include "rtos/semaphore.hpp"
#include "misc/invocation_wrapper.hpp"
#include "modifiers/loggable.hpp"
#include "rtos/thread.hpp"

using namespace std;

namespace RTOS {
/**
 * @brief   Serves as thread starter, creates thread from given parameters
 *          Used when thread needs to be started from IRQ
 */
class Thread_creator : Loggable {
    /**
     * @brief Used for storage of thread parameters
     */
    struct Thread_record {
        /**
         * @brief Construct a new Thread_record object
         */
        Thread_record(){name.reserve(configMAX_TASK_NAME_LEN);};

        /**
         * @brief Construct a new Thread_record object
         *
         * @param name          Name of new thread
         * @param iw            Executable (invocation wrapper) which is used in seed of thread
         * @param stack_size    Stack size of thread
         * @param priority      Priority of thread
         */
        Thread_record(string name, Invocation_wrapper_base<void, void> *iw, uint16_t stack_size, Priority priority) : name(name.substr(0,configMAX_TASK_NAME_LEN-1)), iw(iw), stack_size(stack_size), priority(priority){ };

        string                               name;
        Invocation_wrapper_base<void, void> *iw;
        uint16_t                             stack_size;
        Priority                             priority;
    };

    /**
     * @brief   Size of thread initializer buffer
     *          This is maximum amount of thread which can be created between two run of Thread creator
     *          If there is high amount of IRQ which start threads, this count should be higher
     */
    const unsigned short initializer_size = 3;

    /**
     * @brief   Storage for thread initializers
     */
    vector<Thread_record> thread_initializers{initializer_size};

    /**
     * @brief Index on which show first empty position in thread_initializers
     */
    unsigned short initializer_index_top = 0;

    /**
     * @brief Enables creation of threads from initializers
     */
    RTOS::Semaphore sem = RTOS::Semaphore(1,0);

public:
    /**
     * @brief   Construct a new Thread_creator object
     *          Start thread in which Thread creator loop is running
     */
    Thread_creator();

    /**
     * @brief   Runs in own thread, waits for new initializers to be setup
     */
    void Loop();

    /**
     * @brief   Method called by from IRQ (mostly by IRQ handler)
     *          Saves new initializer fot thread
     *          When thread of Thread creator is then running new threads are created
     *
     * @param name          Name of new thread
     * @param iw            Executable (invocation wrapper) which is used in seed of thread
     * @param stack_size    Stack size of thread
     * @param priority      Priority of thread
     */
    void Add(string name, Invocation_wrapper_base<void, void> *iw, uint16_t stack_size, Priority priority);
};

/**
 * @brief Global instance of Thread Creator
 */
extern Thread_creator Thread_creator_entity;

/**
 * @brief   Pass request to global instance of Thread creator
 *
 * @param name          Name of new thread
 * @param iw            Executable (invocation wrapper) which is used in seed of thread
 * @param stack_size    Stack size of thread
 * @param priority      Priority of thread
 */
void Add_thread(string name, Invocation_wrapper_base<void, void> *iw, uint16_t stack_size = 256, Priority priority = Priority::Normal);

};
