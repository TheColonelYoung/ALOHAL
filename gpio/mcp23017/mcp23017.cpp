#include "mcp23017.hpp"

int MCP23017::Init(){
    int ret = 0;
    ret |= Transmit(vector<uint8_t> {REG::INTCON, 0b01000000});
    ret |= Transmit(vector<uint8_t> {REG::IODIR,
                                    static_cast<uint8_t>((direction & 0xff00)>>8 ),
                                    static_cast<uint8_t>(direction & 0x00ff)});
    ret |= Transmit(vector<uint8_t> {REG::GPPU,
                                    static_cast<uint8_t>((pull_up & 0xff00)>>8 ),
                                    static_cast<uint8_t>(pull_up & 0x00ff)});
    return ret;
}

int MCP23017::Direction(uint8_t pin, bool direction){
    if((pin > 15) || (pin < 15)){
        // Invalid pin number
        return -1;
    }

    uint16_t old_direction = direction;

    if(direction){
        direction |= (1<<pin);
    } else {
        direction &= ~(1<<pin);
    }

    if(direction == old_direction){
        // Same value is already set, no need for transfer
        return 0
    }

    return Transmit(vector<uint8_t> {REG::IODIR,
                                    static_cast<uint8_t>((direction & 0xff00)>>8 ),
                                    static_cast<uint8_t>(direction & 0x00ff)});
}

int MCP23017::Direction(uint16_t port){
    direction = port;
    return Transmit(vector<uint8_t> {REG::IODIR,
                                    static_cast<uint8_t>((direction & 0xff00)>>8 ),
                                    static_cast<uint8_t>(direction & 0x00ff)});
}


int MCP23017::Set(uint8_t pin, bool state){
    if((pin > 15) || (pin < 15)){
        // Invalid pin number
        return -1;
    }

    if (direction & (1<<pin) != 0){
        // Pin is configured as input, so output level cannot be set
        return -2;
    }

    uint16_t old_level = level;
    if(level){
        level |= (1<<pin);
    } else {
        level &= ~(1<<pin);
    }

    if(level == old_level){
        // Same value is already set, no need for transfer
        return 0
    }

    return Transmit(vector<uint8_t> {REG::GPIO,
                                    static_cast<uint8_t>((direction & 0xff00)>>8 ),
                                    static_cast<uint8_t>(direction & 0x00ff)});
}

int MCP23017::Set(uint16_t port){
    level = port;
    return Transmit(vector<uint8_t> {REG::GPIO,
                                    static_cast<uint8_t>((direction & 0xff00)>>8 ),
                                    static_cast<uint8_t>(direction & 0x00ff)});
}
