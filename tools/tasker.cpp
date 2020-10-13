#include "tasker.hpp"

Tasker::Tasker(Timer *timer) :
    Tool("Tasker"),
    timer(timer){
    // Set prescaler to us
    timer->Prescaler_set(timer->Input_frequency() / 1000000);
    // Register IRQ Callback
    timer->IRQ->Register(this, &Tasker::IRQ);
}

void Tasker::_Start(){
    running = true;

    _Set_timer(events.front()->Remaining_time());
    timer->Enable_IRQ();
}

void Tasker::_Stop(){
    timer->Disable_IRQ();
    running = false;
}

void Tasker::_Update(unsigned long time_past){
    if (not time_past) {
        time_past = events.front()->Remaining_time();
    }

    for (auto &event:events) {
        event->Update(time_past);
    }

    for (auto it = events.begin(); it != events.end();) {
        if ((*it)->Remaining_time() <= 0) {
            // Execute event, based on return value it can be last run of event
            if ((*it)->Run()) {
                // Delete event when is depleted
                delete (*it);
                events.erase(it);
                // Descrease iterator, due to removing one element
                it--;
                // Stop Tasker if there are no events
                if (events.size() == 0) {
                    _Stop();
                    return;
                }
            }
        }
        // Increase iterator, move to another element in list
        it++;
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

string Tasker::String(){
    string text = "";
    for (auto &event:events) {
        text += event->String() + "\r\n";
    }
    return text;
}

void Tasker::_Set_timer(unsigned long time){
    timer->Time_set(time);
}

void Tasker::IRQ(){
    _Update();
}
