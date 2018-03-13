#include "endstop.hpp"

bool Endstop::Read(){
    return input_pin.Read();
}

//TODO change registred IRQ
int Endstop::New_pin(Pin new_pin){
    input_pin = new_pin;
}
