#include "component.hpp"

Component::Component(string name):
    name(name), id(id_counter)
{
    id_counter++;

    this->name = device()->Register_component(this);
}

string Component::Name() const{
    return name;
}
