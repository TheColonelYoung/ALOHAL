#include "mcp23017.hpp"

int MCP23017::Init(){
    int ret = 0;
    ret |= Transmit(vector<uint8_t> {REG::INTCON, 0b01000000, 0b01000000});
    ret |= Transmit(vector<uint8_t> {REG::IODIR,
                                    static_cast<uint8_t>(direction & 0x00ff),
                                    static_cast<uint8_t>((direction & 0xff00)>>8)});
    ret |= Transmit(vector<uint8_t> {REG::GPPU,
                                    static_cast<uint8_t>(pull_up & 0x00ff),
                                    static_cast<uint8_t>((pull_up & 0xff00)>>8 )});
    return ret;
}

int MCP23017::Direction(uint8_t pin, bool new_direction){
    if((pin > 15)){
        // Invalid pin number
        return -1;
    }

    uint16_t old_direction = direction;

    if(new_direction){
        direction |= (1<<pin);
    } else {
        direction &= ~(1<<pin);
    }

    if(direction == old_direction){
        // Same value is already set, no need for transfer
        return 0;
    }

    return Transmit(vector<uint8_t> {REG::IODIR,
                                    static_cast<uint8_t>(direction & 0x00ff),
                                    static_cast<uint8_t>((direction & 0xff00)>>8)});
}

int MCP23017::Direction(uint16_t port){
    direction = port;
    return Transmit(vector<uint8_t> {REG::IODIR,
                                    static_cast<uint8_t>(direction & 0x00ff),
                                    static_cast<uint8_t>((direction & 0xff00)>>8)});
}

int MCP23017::Set(uint8_t pin, bool state){
    if((pin > 15)){
        // Invalid pin number
        return -1;
    }

    if ((direction & (1<<pin)) != 0){
        // Pin is configured as input, so output level cannot be set
        return -2;
    }

    if(state){
        level |= (1<<pin);
    } else {
        level &= ~(1<<pin);
    }

    return Transmit(vector<uint8_t> {REG::GPIO,
                                    static_cast<uint8_t>(level & 0x00ff),
                                    static_cast<uint8_t>((level & 0xff00)>>8)});
}

int MCP23017::Set(uint16_t port){
    level = port;
    return Transmit(vector<uint8_t> {REG::GPIO,
                                    static_cast<uint8_t>(level & 0x00ff),
                                    static_cast<uint8_t>((level & 0xff00)>>8)});
}

uint8_t MCP23017::Toggle(uint8_t pin_number){
    bool actual_level = (level>>pin_number) & 1;
    return Set(pin_number, !actual_level);
}

int MCP23017::Pull_up(uint16_t port){
    pull_up = port;
    return Transmit(vector<uint8_t> {REG::GPPU,
                                    static_cast<uint8_t>(pull_up & 0x00ff),
                                    static_cast<uint8_t>((pull_up & 0xff00)>>8)});

}

int MCP23017::IRQ_Enable(Pin *IRQ_input){
    this->IRQ_input = IRQ_input;
    this->IRQ_input->IRQ->Register(this, &MCP23017::IRQ_event);

    return 0;
}

int MCP23017::IRQ_setup(uint8_t pin, Pin_IRQ::Trigger trigger){
    if((pin > 15)){
        // Invalid pin number
        return ENOTTY;
    }

    if((direction & (1<<pin)) == 0){
        // Selected pin is not configured as input
        return EIO;
    }

    irq_triggers[pin] = trigger;

    uint16_t int_en = 0;
    uint16_t intcon = 0;
    uint16_t defval = 0;

    for(int i = 0; i < 16; i++){
        if (irq_triggers[i] == Pin_IRQ::Trigger::Disabled){
            int_en &= ~(1<<i);
        } else if (irq_triggers[i] == Pin_IRQ::Trigger::On_change){
            int_en |= (1<<i);
            intcon &= ~(1<<i);
        } else if (irq_triggers[i] == Pin_IRQ::Trigger::Falling_edge){
            int_en |= (1<<i);
            intcon |= (1<<i);
            defval |= (1<<i);
        } else if(irq_triggers[i] == Pin_IRQ::Trigger::Rising_edge){
            int_en |= (1<<i);
            intcon |= (1<<i);
            defval &= ~(1<<i);
        }
    }

    Transmit_16b(REG::GPINTEN, int_en);
    Transmit_16b(REG::INTCON, intcon);
    Transmit_16b(REG::DEFVAL, defval);

    return 0;
}

void MCP23017::IRQ_event(){
    // Perform reading from
    Transmit(vector<uint8_t> {REG::INTF});

    // Contains which pin caused interrupt
    auto received = Receive(2);

    // check all bits in both registers, if some is set notify IRQ handler
    for(int byte_position = 0; byte_position < 2; byte_position++){
        for(int bit_position = 0; bit_position < 8; bit_position++){
            if ((received[byte_position] & (1<<bit_position)) > 0){
                IRQ.Notify(bit_position *byte_position);
            }
        }
    }
}

vector<uint8_t> MCP23017::Memory_dump(){
    Transmit(vector<uint8_t> {0x00});
    return Receive(22);
}

int MCP23017::Transmit_16b(REG target_register, uint16_t data){
    return Transmit(vector<uint8_t> {   static_cast<uint8_t>(target_register),
                                        static_cast<uint8_t>(data & 0x00ff),
                                        static_cast<uint8_t>((data & 0xff00)>>8)});
}
