#include "tasker.hpp"

Tasker::Tasker(Timer *timer) : timer(timer){
    // Set prescaler to us
    timer->Prescaler_set(timer->Input_frequency()/1000000);
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
    if(not time_past){
        time_past = events.front()->Remaining_time();
    }

    //device()->cli->Print(to_string(device()->mcu->TIM_2->Counter_get() ) + "\r\n");

    for (auto &event:events) {
        event->Update(time_past);
    }

    for (auto &event:events) {
        if (event->Remaining_time() <= 0) {
            if (event->Run()) {
                // Delete event when is depleted
                delete events.front();
                events.pop_front();
                if(events.size() == 0){
                    _Stop();
                    return;
                }
            }
        }
    }

    _Sort();

    device()->cli->Print(String());

    _Set_timer(events.front()->Remaining_time());
}

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
