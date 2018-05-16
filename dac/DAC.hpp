#ifndef DAC_H
#define DAC_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif

#include <vector>

#include "globals.hpp"

using namespace std;

class DA_C;

class DA_C_channel {
    DA_C *_parent_convertor;
    uint _index;

public:
    DA_C_channel() = default;
    DA_C_channel(DA_C *parent, int index);

    void Start();
    int Set_value(float voltage);
    int Set_reference(float value);
    void Stop();
};

class DA_C {


public:
    DAC_HandleTypeDef *_handler;

private:
    uint _channel_count;

public:
    float supply_voltage = -1;
    vector<DA_C_channel> channel;

public:
    DA_C() = default;
    DA_C(DAC_HandleTypeDef *handler, int channel_count);
    DA_C(DAC_HandleTypeDef *handler, int channel_count, float supply_voltage);

    DAC_HandleTypeDef *Handler();

    void Test();

    void Calibrate();
};


#endif // ifndef DAC_H
