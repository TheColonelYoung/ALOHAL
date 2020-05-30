/**
 * @file logger.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 29.04.2020
 */
#pragma once

using namespace std;

#include <vector>
#include <string>

#include "misc/invocation_wrapper.hpp"
#include "device/device.hpp"

enum class Log_levels{
    Fatal,
    Error,
    Warning,
    Notice,
    Debug
};

/**
 * @brief   Singleton class used for logging
 *          Contain logging channels, this can be method of any object with corrent interface
 */
class Logger
{
private:
    /**
     * @brief Pointer to singleton instance
     */
    inline static Logger* instance = nullptr;

    /**
     * @brief All channels to which will be messages logged
     */
    vector<Invocation_wrapper_base<int, string> *> logging_channels;
    
    /**
     * @brief  Suffix which will be added to record, and of the line is preferred
     */
    const string record_suffix = "\r\n";
    
    /**
     * @brief   Actual threshold for records, record with lower level of importance are ignored
     */
    Log_levels log_level = Log_levels::Debug;
    
public:
    Logger() = default;
    ~Logger() = default;
    
    /**
     * @brief Singleton instance
     * 
     * @return Logger* One and only instance of logger
     */
    static Logger* Instance();
    
    /**
     * @brief   Add output channel to which messages are logged
     * 
     * @tparam class_T      Target class
     * @param new_channel   Wrapper in which is object and his method, arg of method must be string, returning int
     * @return int          Number of actual logging channels
     */
    template <typename class_T>
    int Add_log_channel(Invocation_wrapper<class_T, int, string> *new_channel){
        logging_channels.emplace_back(new_channel);
        return logging_channels.size();
    }
    
    /**
     * @brief Write record to all available log channels
     * 
     * @param record    Text to write to log
     * @return int      Number of channels to which was message logged
     */
    int Log_record(string record); const
    
    /**
     * @brief Set threshold level of importance for messages
     * 
     * @param new_level     Threshold level, record with lower are ignored
     */
    void Set_log_level(Log_levels new_level) {log_level = new_level;};
    
    /**
     * @brief Return actual log level threshold
     * 
     * @return Log_levels   Log level threshold
     */
    Log_levels Level() const { return log_level;};
};

