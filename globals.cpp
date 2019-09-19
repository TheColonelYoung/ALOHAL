#include "globals.hpp"

#include "device/device.hpp"

Device* device(){
    return Device::Instance();
}
