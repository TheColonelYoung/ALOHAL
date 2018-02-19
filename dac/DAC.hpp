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

// entened pointer to DAC from main
extern DAC_HandleTypeDef hdac;

// value of voltage connected to VREF
#define DAC_REF_VOLTAGE 3.2
// number of DAC on this chip
#define NUM_OF_DAC      2

/**
 * Enable and start the DAC, without this cannot be value set
 * @param  channel number of channel restricted by NUM_OF_DAC
 * @return         return channel number if was succesfull or -1 if not
 */
int DAC_Start(int channel);

/**
 * Stop the DAC (saves power), values cannot be writen
 * @param  channel number of channel restricted by NUM_OF_DAC
 * @return         return channel number if was succesfull or -1 if not
 */
int DAC_Stop(int channel);

/**
 * Set voltage value to pin of DAC
 * @param  channel number of channel restricted by NUM_OF_DAC
 * @param  voltage output voltage on pin restricted by DAC_REF_VOLTAGE
 * @return         return value in DAC register if was succesfull or -1 if not
 */
int DAC_set_value(int channel, float voltage);

#endif // ifndef DAC_H
