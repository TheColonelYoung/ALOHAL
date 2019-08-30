#include "component.hpp"

Component::Component(string name){
    id = id_counter;
    id_counter++;

    device->Register_component(shared_from_this());
}

string Component::Name() const{
    return name;
}
