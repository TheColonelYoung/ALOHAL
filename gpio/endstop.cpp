#include "endstop.hpp"

Endstop::Read(){
    return input_pin.Read();
}
