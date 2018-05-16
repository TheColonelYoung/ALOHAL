#include "current_supply.hpp"

Current_supply::Current_supply(DA_C_channel *DAC_chan, uint sense) : _DAC_chan(DAC_chan),_sense_resistor(sense){

}

Current_supply::Current_supply(DA_C_channel *DAC_chan, uint sense, uint limit) : _limit(limit){
    Current_supply(DAC_chan, sense);
}

int Current_supply::Set_current(uint current_ua){
    _DAC_chan->Set_value(_sense_resistor * (current_ua / 1000000.0));
    return current_ua;
}
