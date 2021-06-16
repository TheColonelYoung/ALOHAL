/**
 * @file semaphore.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 14.06.2021
 */
#pragma once

#include "cmsis_os2.h"

namespace RTOS{
    /**
     * @brief Wrapper class to synchronization semaphore
     */
    class Semaphore {
    public:
        /**
         * @brief Internal semaphore structure
         */
        osSemaphoreId_t semaphore;

        /**
         * @brief Construct a new Semaphore object
         *
         * @param max_tokens    Maximum of token
         * @param init_tokens   Initial amount of tokens
         */
        Semaphore(uint32_t max_tokens = 1, uint32_t init_tokens = 1){
            semaphore = osSemaphoreNew(max_tokens, init_tokens, NULL);
        }

        /**
         * @brief   Remove one token to semaphore
         *          If semaphore has zero tokens, then waits forever for token
         */
        inline void Acquire() const{
            osSemaphoreAcquire(semaphore, osWaitForever);
        }

        /**
         * @brief Give one token to semaphore
         */
        inline void Release() const{
            osSemaphoreRelease(semaphore);
        }
    };
}
