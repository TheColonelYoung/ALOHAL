#ifndef ADC_H
#define ADC_H

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

#include "gpio/pin.hpp"
#include "usb/USB_VCP.hpp"

#define ADC_REF_VOLTAGE 	2.89			//voltage of external reference
#define VREFINT_CAL_ADDR    0x1FFFF7BA

//uint16_t VREFINT_CAL = *((uint16_t*)VREFINT_CAL_ADDR); // VREFINT calibration value


#endif
