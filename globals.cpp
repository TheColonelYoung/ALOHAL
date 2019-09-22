#include "globals.hpp"

#include "device/device.hpp"

Device * const device(){
    return Device::Instance();
}
