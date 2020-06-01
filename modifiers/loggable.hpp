/**
 * @file loggable.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 29.04.2020
 */
#pragma once

/**
 * @brief Extends log message method, add function names, source filename and line to logging output
 */
// #define Log(level, record) Log_record(level, record, string(__FUNCTION__), string(__FILE__), __LINE__);

using namespace std;

#include <string>

#include "misc/logger.hpp"

/**
 * @brief   Modifier class, class derivated from this can log messages to Logger
 *          Inherited class receive method of this class for logging
 *          For gathering more information use macro above, macro only simplify source code line for logging
 */
class Loggable
{
public:
    Loggable() = default;
    ~Loggable() = default;
    
    /**
     * @brief   Logs message with given level via Logger
     *          Level must be higher then Logger threshold
     * 
     * @param level     Level of importance, see Logger class
     * @param record    Message to log
     * @param method    Method from which is this logging called, macro Log
     * @param file      Filename from which is this logging called, macro Log
     * @param line      Line from which is this logging called, macro Log
     * @return int      0 if log level is lower the Logger threshold
     *                  higher then 0 is size of record is logged
     */
    virtual int Log_record(Log_levels level, string record, string method, string file, unsigned int line);
    
    virtual int Log(Log_levels level, string record);
};
