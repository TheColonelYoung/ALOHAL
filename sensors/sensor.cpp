#include "sensor.hpp"

Sensor::Sensor() :
    Component("Unknown_sensor")
{ }

Sensor::Sensor(string name) :
    Component(name)
{ }

vector<string> Sensor::Measurable_quantities() const {
    vector<string> all_quantities = Measurable_numerical();
    all_quantities.insert(all_quantities.end(), Measurable_text().begin(), Measurable_text().end() );
    all_quantities.insert(all_quantities.end(), Measurable_others().begin(), Measurable_others().end() );
    return all_quantities;
}

vector<string> Sensor::Measurable_numerical() const {
    vector<string> measurable_numerical;
    for (auto &quantity:numerical_quantity) {
        measurable_numerical.emplace_back(quantity.first);
    }
    return measurable_numerical;
}

vector<string> Sensor::Measurable_text() const {
    vector<string> measurable_text;
    for (auto &quantity:text_quantity) {
        measurable_text.emplace_back(quantity.first);
    }
    return measurable_text;
}

vector<string> Sensor::Measurable_others() const {
    return other_quantity;
}

optional<double> Sensor::Measure_numerical(string quantity){
    if (numerical_quantity.find(quantity) != numerical_quantity.end()){
        return numerical_quantity[quantity]->Invoke();
    }else{
        return {};
    }
}

optional<string> Sensor::Measure_text(string quantity){
    if (text_quantity.find(quantity) != text_quantity.end()){
        return text_quantity[quantity]->Invoke();
    }else{
        return {};
    }
}

bool Sensor::Quantity_exists(string quantity_name){
    bool exists = false;
    if (numerical_quantity.find(quantity_name) != numerical_quantity.end()){
        exists = true;
    } else if (text_quantity.find(quantity_name) != text_quantity.end()){
        exists = true;
    } else if (find(other_quantity.begin(), other_quantity.end(), quantity_name) != other_quantity.end()){
        exists = true;
    }
    return exists;
}
