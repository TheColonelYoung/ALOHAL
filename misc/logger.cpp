#include "logger.hpp"

Logger* Logger::Instance(){
    if (instance == nullptr){
        instance = new Logger();
    }
    return instance;
}

int Logger::Log_record(string record){
    for(auto &channel:logging_channels){
        channel->Invoke(record + record_suffix);
    }
    return logging_channels.size();
}
