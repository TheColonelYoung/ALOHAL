#include "planner.hpp"

#include "device/device.hpp"

Planner::Planner(string name, int phase_count):
    name(name){
    phases.resize(phase_count+2);
}

Planner::~Planner(){

}

int Planner::Add_phase(){
    phases.resize(phases.size()+1);
    return phases.size();
}

void Planner::Start(){
    running = true;
}

void Planner::Stop(){
    running = false;
}

void Planner::Done(){
    Stop();
    Unregister();
}

int Planner::Register(){
    int ret = device()->Register_planner(this);
    if(ret < 1){
        //Registration failed
        return EEXIST;
    }
    registred = true;
    return 0;
}

int Planner::Unregister(){
    int ret = device()->Unregister_planner(this);
    if(ret == -1){
        // Planner with this name probably not exists
        return EFAULT;
    }
    registred = false;
    return 0;
}
