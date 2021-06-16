#include "tasker_event.hpp"

Tasker_event::Tasker_event(string name, unsigned int repetition, unsigned long period, Invocation_wrapper_base<void, void> *execution_target, unsigned short priority) :
    name(name),
    execution_target(execution_target),
    repetition(repetition),
    period(period),
    remaning_time(period),
    priority(priority)
{ }

Tasker_event::~Tasker_event(){
    if (execution_target) {
        delete execution_target;
    }
}

bool Tasker_event::Run(){
    remaning_time = period;

    if(repetition > 0){
        repetition--;
    } else if (repetition == 0){
        return true;
    }

    execution_target->Invoke();

    if (repetition != 0){
        return false;
    }

    return true;
}
