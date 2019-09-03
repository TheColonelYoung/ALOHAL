#include "component.hpp"

Component::Component(string name){
    id = id_counter;
    id_counter++;

    device->Register_component(this);
}

string Component::Name() const{
    return name;
}
