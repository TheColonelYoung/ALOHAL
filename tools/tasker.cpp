#include "tasker.hpp"

Tasker::Tasker(Timer *timer) :
    Tool("Tasker"),
    timer(timer){
    // Set prescaler to us
    timer->Prescaler(timer->Input_frequency() / 1000000);
    // Register IRQ Callback
    timer->Mode(Timer::Modes::Timer_IRQ);
    timer->IRQ->Register(this, &Tasker::IRQ);
    timer->Optimize(true);
}

Tasker* Tasker::Instance(Timer *timer){
    if (instance == nullptr){
        instance = new Tasker(timer);
    }
    return instance;
}

Tasker* Tasker::Instance(){
    return instance;
}

void Tasker::_Start(){
    running = true;

    _Set_timer(events.front()->Remaining_time());
    timer->Start();
}

void Tasker::_Stop(){
    timer->Stop();
    running = false;
}

void Tasker::_Update(unsigned long time_past){
    if (not time_past) {
        time_past = events.front()->Remaining_time();
    }

    for (auto &event:events) {
        event->Update(time_past);
    }

    for (auto event_it = events.begin(); event_it != events.end();) {
        if ((*event_it)->Remaining_time() <= 0) {
            // Execute event, based on return value it can be last run of event
            if ((*event_it)->Run()) {
                // Delete event when is depleted
                delete (*event_it);
                events.erase(event_it);
                // Descrease iterator, due to removing one element
                event_it--;
                // Stop Tasker if there are no events
                if (events.size() == 0) {
                    _Stop();
                    return;
                }
            }
        }
        // Increase iterator, move to another element in list
        event_it++;
    }

    _Sort();

    _Set_timer(events.front()->Remaining_time());
} // Tasker::_Update

void Tasker::_Sort(){
    events.sort(
        [](Tasker_event *event_1, Tasker_event *event_2){
        return *event_1 < *event_2;
    }
    );
}

void Tasker::_Set_timer(unsigned long time){
    timer->Time_set(time);
}

void Tasker::IRQ(){
    _Update();
}
