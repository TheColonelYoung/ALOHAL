#include "stats.hpp"

uint32_t RTOS::Thread_count(){
    return uxTaskGetNumberOfTasks();
}

uint32_t RTOS::Free_memory(){
    return xPortGetFreeHeapSize();
}

uint32_t RTOS::Minimum_memory(){
    return xPortGetMinimumEverFreeHeapSize();
}

string RTOS::Memory_summary(){
    HeapStats_t stats;
    vPortGetHeapStats(&stats);
    string text;
    text.reserve(150);
    text += "Total memory:\t\t " + to_string(configTOTAL_HEAP_SIZE) + "\r\n";
    text += "Available memory:\t " + to_string(stats.xAvailableHeapSpaceInBytes) + "\r\n";
    text += "Minimum memory:\t\t " + to_string(stats.xMinimumEverFreeBytesRemaining) + "\r\n";
    text += "Allocations:\t\t " + to_string(stats.xNumberOfSuccessfulAllocations) + "\r\n";
    text += "Deallocations:\t\t " + to_string(stats.xNumberOfSuccessfulFrees) + "\r\n";
    text += "Free blocks available:\t " + to_string(stats.xNumberOfFreeBlocks) + "\r\n";
    text += "Largest block available: " + to_string(stats.xSizeOfLargestFreeBlockInBytes) + "\r\n";
    return text;
}

string RTOS::Thread_summary(){
    uint16_t thread_count = uxTaskGetNumberOfTasks();
    vector<TaskStatus_t> thread_handles{thread_count};
    uint32_t runtime;

    auto size = uxTaskGetSystemState(thread_handles.data(), thread_count, &runtime);

    if(size != thread_count){
        return "Error";
    }

    string text;
    text += string(80, '-') + "\r\n";
    text += "Name\t\t\tState\t\tStack_min_free\tCPU_Usage\r\n";
    text += string(80, '-') + "\r\n";
    for(auto &task:thread_handles){
        string name = string(task.pcTaskName);
        text += name;
        int fill = 3-(name.length()/8);
        text += string(fill, '\t');
        text += Task_state_translation.at(task.eCurrentState);
        //uint16_t stack_size = uxTaskGetStackSize(task.xHandle);
        //text += "\t\t" + to_string(stack_size);
        text += "\t\t" + to_string(uxTaskGetStackHighWaterMark(task.xHandle) * 4);
        text += "\t\t" + to_string((task.ulRunTimeCounter/(double)runtime) * 100) + "%";
        text += "\r\n";
    }
    return text;
}
