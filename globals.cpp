#include "globals.hpp"

#include "device/device.hpp"
#include "tools/tasker.hpp"

Device * const device(){
    return Device::Instance();
}

Tasker * const tasker(){
    return Tasker::Instance();
}
