#include "LED_Driver.hpp"

vector<LED_Driver> LED_Driver_stash;

LED_Driver::LED_Driver(int channel_set, Pin enable_pin_set, float current_limit_ma_set, float multiplicator_set)
{
    channel = channel_set;
    current_limit_ma = current_limit_ma_set;
    multiplicator = multiplicator_set;

    enable_pin = enable_pin_set;

    enable_pin.Set(0);

    DAC_Start(channel_set);
    DAC_set_value(channel_set, 0.0);

    LED_Driver_stash.push_back(*this);
}

void LED_Driver_Init()
{
    LED_Driver(1, Pin('C', 9), 25, 3);
    LED_Driver(2, Pin('A', 8), 25, 1);
}

void LED_Driver::Set_value(float value)
{
    if (value > 0) {
        enable_pin.Set(1);
    } else  {
        enable_pin.Set(0);
    }

    if (value > current_limit_ma) {
        return;
    }

    DAC_set_value(channel, (value / 100.0) * multiplicator);
}

float LED_Driver_refine(float orig_value)
{
    float new_value = orig_value;
    return new_value;
}
