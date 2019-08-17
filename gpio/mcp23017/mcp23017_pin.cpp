#include "mcp23017_pin.hpp"

Pin_MCP23017::Pin_MCP23017(MCP23017 &expander, uint8_t pin_number):
    Pin(0, pin_number), expander(&expander){
}

void Pin_MCP23017::Toggle(){
    expander->Toggle(pin_number);
}

void Pin_MCP23017::Set(bool value){
    expander->Set(pin_number, value);
}

bool Pin_MCP23017::Read(){
    //return expander->Read(pin_number);
    return false;
}
