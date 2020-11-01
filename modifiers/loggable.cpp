#include "loggable.hpp"

int Loggable::Log_record(Log_levels level, string record, string method, string file, unsigned int line){

    string class_name = typeid(*this).name();

    string position = "[" + file + "::" + to_string(line) + "]";
    string prefix = position + "[" + class_name + "::" + method + "] ";

    record = prefix + record;

    if (Logger().Instance()->Level() >= level){
        Logger().Instance()->Log_record(record);
    } else {
        return 0;
    }
    return record.length();
}

int Loggable::Log(Log_levels level, string record){
    if (Logger().Instance()->Level() >= level){
        Logger().Instance()->Log_record(record);
    } else {
        return 0;
    }
    return record.length();
}
