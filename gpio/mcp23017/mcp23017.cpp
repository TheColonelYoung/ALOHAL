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

