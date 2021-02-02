#pragma once

#include <string>
#include <tuple>

#include "irq_handler.hpp"

using namespace std;

template <typename identifier_type>
class IRQ_multi_handler {
public:

    vector<tuple<identifier_type, IRQ_handler> > handlers;

    IRQ_multi_handler() = default;

    template <typename registrator_class>
    void Register(identifier_type identifier, registrator_class& object, void (registrator_class::*method_pointer_set)()){
        for (auto &handler_tuple : handlers) {
            if (get<0>(handler_tuple) == identifier) {
                get<1>(handler_tuple).Register(object, method_pointer_set);
                return;
            }
        }

        IRQ_handler new_handler = IRQ_handler();
        new_handler.Register(object, method_pointer_set);
        handlers.emplace_back(make_tuple(identifier, new_handler));
    }

    //NOTE there should be a problem, registration of pointer to object
    template <typename registrator_class>
    void Register(identifier_type identifier, registrator_class* object, void (registrator_class::*method_pointer_set)()){
        for (auto &handler_tuple : handlers) {
            if (get<0>(handler_tuple) == identifier) {
                get<1>(handler_tuple).Register(object, method_pointer_set);
                return;
            }
        }

        IRQ_handler new_handler = IRQ_handler();
        new_handler.Register(object, method_pointer_set);
        handlers.emplace_back(make_tuple(identifier, new_handler));
    }

    void Register(identifier_type identifier, void (*function_pointer_set)(void)){
        for (auto &handler_tuple : handlers) {
            if (get<0>(handler_tuple) == identifier) {
                get<1>(handler_tuple).Register(function_pointer_set);
                return;
            }
        }

        IRQ_handler new_handler = IRQ_handler();
        new_handler.Register(function_pointer_set);
        handlers.emplace_back(make_tuple(identifier, new_handler));
    }

    int Notify(identifier_type identifier){
        for (auto &handler_tuple : handlers) {
            if (get<0>(handler_tuple) == identifier) {
                get<1>(handler_tuple).Notify();
                return 1;
            }
        }
        return -1;
    }
};
