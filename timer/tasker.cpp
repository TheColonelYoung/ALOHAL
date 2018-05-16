#include "tasker.hpp"

Tasker::Tasker(){ }

int Tasker::Start(){
    delay_time   = -1;
    tasker_state = true;
    _Set_IT(100);
    HAL_TIM_Base_Start_IT(&TASKER_TIMER);

    return 0;
}

int Tasker::Stop(){
    tasker_state = false;
    HAL_TIM_Base_Stop_IT(&TASKER_TIMER);
    return 0;
}

void Tasker::Update(){
    long timer_value = TIM_get_time(&TASKER_TIMER);

    if (timer_value > events[0].remaining_time) {
        timer_value = events[0].remaining_time - 1;
    }

    for (auto &event : events) { // decrease time by delay time and repeat counter
        if (event.repeat != 0) {
            event.remaining_time -= timer_value;
        }
    }

    delay_time = events[0].remaining_time;
    _Set_IT(delay_time);
    __HAL_TIM_SET_COUNTER(&TASKER_TIMER, 0);
}

int Tasker::Create_event(string name, int time, string unit, int repeat, void (*function)(void), int priority){
    int multiplier = 1; // time calculation based on time and unit
    if (unit == "us") {
        multiplier = 1;
    } else if (unit == "ms") {
        multiplier = 1000;
    } else if (unit == "s") {
        multiplier = 1000000;
    } else if (unit == "min") {
        multiplier = 60000000;
    } else {
        return -1;
    }
    long n_time = time * multiplier;

    if (name.length() > 32) { // name validity check
        return -1;
    }

    if (priority > 10 || priority < 0) { // priority check
        return -1;
    }

    Event tmp_event = Event(name, n_time, repeat, function, priority);

    free_ID++;

    int index = _Event_index(tmp_event);

    events.push_back(tmp_event); // pushing new event to back

    number_of_events++;

    if (tasker_state) {
        Update();
    }
    return 0;
} /* tasker::create_event */

void Tasker::Handle_event(){
    // Initial handle after starting interupt
    if (delay_time == -1) {
        delay_time = events[0].remaining_time;
        delay_time = 100000;
        _Set_IT(delay_time);
        return;
    }
    // regular handeling - routine
    for (auto &event : events) { // decrease time by delay time and repeat counter
        if (delay_time != 0) {
            if (event.repeat != 0) {
                event.remaining_time -= delay_time;
            }
        }
    }
    // error handeling - timer or interupt is broken
    if (events[0].remaining_time != 0) {
        return ; // something gone horrybly wrong
        // regular handeling - processing event
    } else {
        _actual_event = events[0].name; // set actual event;
        (*(events[0].function))();      // run event function
        _actual_event = "None";         // erase actual event;
        // decresing repeat of event
        if (!(events[0].repeat == -1 || events[0].repeat == 0)) {
            events[0].repeat -= 1;
        }

        events[0].remaining_time += events[0].time;                                           // add time to event
        rotate(events.begin(), events.begin() + 1, events.begin() + _Event_index(events[0])); // shift event to new position
    }
    // recursive handeling of synchronized events
    if (events[0].remaining_time == 0) { // if more than one events on same time has remaning time 0
        delay_time = 0;
        Handle_event();
    }
    // preparing for next interupt
    delay_time = events[0].remaining_time;
    _Set_IT(events[0].remaining_time); // set time for new interrupt
    return;
} /* Tasker::Handle_event */

int Tasker::Event_count(){
    return number_of_events;
}

int Tasker::Get_freeID(){
    return free_ID;
}

void Tasker::Send_events(){
    /*
    string message;

    message = itoa((int) events.size()) + "\n";
    mes     = message.c_str();
    uart_send_string(huart2, (char *) mes);

    for (auto &event : events) {
        message = event.name + ":" + itoa(event.repeat) + ":" + itoa((int) event.remaining_time) + ":" + itoa((int) event.time) + ":" + itoa(event.id) + "\n";
        mes     = message.c_str();
        uart_send_string(huart2, (char *) mes);
    }
    message = "Event STOP\n";
    mes     = message.c_str();
    uart_send_string(huart2, (char *) mes);*/
}

int Tasker::Pause_event(string event_name){
    for (auto &event : events) {
        if (event.name == event_name) {
            event.repeat = 0;
        }
    }
    return -1;
}

int Tasker::Resume_event(string event_name, int event_repeat, int delay_us){
    for (auto &event : events) {
        if (event.name == event_name) {
            event.repeat = event_repeat;
            if (delay_us == 0) {
                event.remaining_time = event.time;
            } else {
                event.remaining_time = delay_us;
            }
        }
    }
    return -1;
}

string Tasker::Get_actual_event(){
    return _actual_event;
}

int Tasker::_Event_index(Event tmp_event){
    if (number_of_events == 0) { // first event
        return 0;
    } else {
        if (tmp_event.repeat == 0) {
            return events.size();
        }
        int pos = 0;
        for (auto &event : events) {
            if (event.repeat == 0) {
                return pos;
            } else if (event.remaining_time < tmp_event.time) {
                pos++;
            } else if (event.remaining_time == tmp_event.time && event.priority >= tmp_event.priority) {
                pos++;
            } else if (event.remaining_time == tmp_event.time && event.priority < tmp_event.priority) {
                return pos;
            } else if (event.remaining_time > tmp_event.time) {
                return pos;
            }

            if (event.id == events.back().id) { // last event
                return pos;
            }
        }
    }
    return -1;
}

void Tasker::Sync_events(string base, string sync){
    if (tasker_state) {
        Update();
    }
    for (auto &event1 : events) {
        for (auto &event2 : events) {
            if (event1.name == base && event2.name == sync) {
                event2.remaining_time = event1.remaining_time;
            }
        }
    }
    if (tasker_state) {
        Update();
    }
}

void Tasker::_Set_IT(long time){
    TIM_6.Time_set(time);
}


Event::Event(string n_name, long n_time, int n_repeat, void(*n_function)(void), int n_priority){
    id   = OTS.Get_freeID();
    name = n_name;
    remaining_time = n_time;
    time     = n_time;
    repeat   = n_repeat;
    function = n_function;
    priority = n_priority;
}
