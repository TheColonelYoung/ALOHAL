#include "planner_phase.hpp"

Planner_phase::Planner_phase(int phase_number) :
    phase_number(phase_number){ }

int Planner_phase::Add_trigger(int trigger_id){
    if (triggers.find(trigger_id) == triggers.end()) {
        // trigger is already contained
        return EEXIST;
    }
    // Create new trigger
    triggers.insert(make_pair(trigger_id, false));
    return 0;
}

void Planner_phase::Trigger(int trigger_id, bool state){
    if (triggers.find(trigger_id) != triggers.end()) {
        // trigger with this ID not exists in this phase
        return;
    }
    triggers[trigger_id] = state;
    for (auto& [key, value]: triggers) {
        if (!value) {
            return;
        }
    }
    // Phase done, run all consequences
    for (auto &consequence:consequences) {
        consequence->Invoke();
    }
}
