
#pragma once

#include "mcu/mcu.hpp"

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L

using namespace std;

class Device{
private:

public:
    MCU mcu = MCU();

    Device() = default;

    void Init();
};

#include "gpio/pin.hpp"
