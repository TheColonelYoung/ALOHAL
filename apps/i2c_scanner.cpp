#include "i2c_scanner.hpp"

I2C_scanner::I2C_scanner(I2C_master *bus) :
    Application("I2C_scanner"),
    bus(bus){ }

int I2C_scanner::Init(){
    return 0;
}

int I2C_scanner::Run(vector<string> &args){
    if (args.size() != 1) {
        device()->cli->Print("No args are allowed\r\n");
        return -1;
    }

    unsigned int device_count = 0;
    device()->cli->Print("Started scanning of I2C bus...\r\n");
    for (size_t address = 0; address < 127; address++) {
        if (bus->Ping(static_cast<uint8_t>(address<<1))) {
            device()->cli->Print("Device found on address: 0x" + dec2hex(address) + "\r\n");
            device_count++;
        }
    }
    device()->cli->Print("Found " + to_string(device_count) + " device\r\n");
    return device_count;
}

int I2C_scanner::Input_load(string input){
    return 0;
}
