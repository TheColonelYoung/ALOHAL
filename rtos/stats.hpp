/**
 * @file stats.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.06.2021
 */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "globals.hpp"

using namespace std;

namespace RTOS{

    /**
     * @brief Translation table from thread state to name of state (string)
     */
    static const map<eTaskState, string> Task_state_translation{
        {eRunning,"Running"},
        {eReady,"Ready"},
        {eBlocked,"Blocked"},
        {eSuspended,"Suspended"},
        {eDeleted,"Deleted"},
        {eInvalid,"Invalid"}
    };

    /**
     * @brief Determinates number of threads in all states in system
     *
     * @return uint32_t Number of threads
     */
    uint32_t Thread_count();

    /**
     * @brief Determinates amount of free memory in system heap
     *
     * @return uint32_t Amount of bytes which are vailable on heap
     */
    uint32_t Free_memory();

    /**
     * @brief Determinates lowest amount of memory which was available during system run
     *
     * @return uint32_t System lowest amount of memory
     */
    uint32_t Minimum_memory();

    /**
     * @brief   Compose Memory statistics
     *          Contains: Size of memory, available memory, lowest amount of memory available, allocation and free count
     *
     * @return string   Statistics of memory
     */
    string Memory_summary();

    /**
     * @brief   Compose Thread statistics for all threads
     *          Contains: Stack size, stack usage, status, CPU usage
     *
     * @return string   Statistics of Threads
     */
    string Thread_summary();
};
