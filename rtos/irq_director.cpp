#include "irq_director.hpp"

#include "irq/irq_handler.hpp"

void RTOS::IRQ_director::Signal(uint16_t irq_id){
    uint16_t event_group_index = irq_id / 31;
    if (event_groups.size() > event_group_index) {
        osEventFlagsSet(event_groups[event_group_index], 1 << (irq_id % 31));
    }
}

void RTOS::IRQ_director::Wait(uint16_t irq_id){
    uint16_t event_group_index = irq_id / 31;
    if(event_groups.size() < event_group_index+1){
        event_groups.resize(event_group_index+1);
    }
    if (event_groups[event_group_index] == nullptr) {
        event_groups[event_group_index] = osEventFlagsNew(nullptr);
    }
    osEventFlagsWait(event_groups[event_group_index], 1 << (irq_id % 31), osFlagsWaitAny, osWaitForever);
}

namespace RTOS{
    RTOS::IRQ_director IRQ_DIR;
}

void RTOS::IRQ_Signal(IRQ_handler *irq){
    RTOS::IRQ_DIR.Signal(irq->ID());
}

void RTOS::IRQ_Wait(IRQ_handler *irq){
    RTOS::IRQ_DIR.Wait(irq->ID());
}
