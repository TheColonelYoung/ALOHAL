#ifndef ADC_H
#define ADC_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
# include "stm32f0xx_hal_adc.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif // ifdef STM32_F0

using namespace std;

#include <cstdint>
#include <string>
#include <memory>

#include "gpio/pin.hpp"
#include "globals.hpp"

#define ADC_REF_VOLTAGE  2.89 // voltage of external reference
#define VREFINT_CAL_ADDR ((uint16_t *)((uint32_t) 0x1FFFF7BA))

#define ADC_CHANNEL_CONCAT(channel) (ADC_CHANNEL_ ## channel)

// uint16_t VREFINT_CAL = *((uint16_t*)VREFINT_CAL_ADDR); // VREFINT calibration value
// VDD = 3.3 * (VREFINT_CAL / VREFINT_DATA)

class AD_C {
    ADC_HandleTypeDef *handler;

    int actual_channel = -1;

    uint16_t reference_val; // Actual reference value
    uint16_t reference_cal; // Reference value for 3.3V
    uint16_t ADC_val;       // Value measured by ADC

    uint supply_voltage = 3300; // in milivolts
    uint resolution = 12;


public:
    AD_C() = default;
    AD_C(ADC_HandleTypeDef *handler);

    int Set_channel(int chan);          // Set actual channel, allowed values: 0-15
    int Set_channel(string chan);       // Allowed values: VREF(16), TEMP(17), VBAT(18)
    int Set_resolution(int resolution);

    void Set_value(uint16_t value); // Set measured value

    uint16_t Measure(); // Rin one pulse measuring ! polling => blocking
    void Start_IT();    // Start one measuring with interrupt

    uint Calibration(); // Measure supply voltage and save as reference

    uint Supply_voltage(); // Supply voltage in miliVolts;
    uint16_t Value();      // Measured value in register

    float Voltage_v(); // Measured voltage in Volts
    uint Voltage();    // In miliVolts
    uint Voltage_u();  // In microVolts
};

#endif // ifndef ADC_H
