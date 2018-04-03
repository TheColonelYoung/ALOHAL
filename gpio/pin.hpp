#ifndef PIN_H
#define PIN_H

// ------------------LIBRARY_OPTIONS----------------
// ------------------LIBRARY_OPTIONS----------------
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

#include "pin_irq.hpp"

#define PORT_SIZE         ((int) GPIOB - (int) GPIOA)
#define ASCII_BASE        (65)
#define PORT_START_ADRESS ((int) GPIOA)

#define pin_set_m(port_name, pin_number, value) HAL_GPIO_WritePin(GPIO ## port_name, GPIO_PIN_ ## pin_number, value ? GPIO_PIN_RESET : GPIO_PIN_SET);
#define pin_tog_m(port_name, pin_number)        HAL_GPIO_TogglePin(GPIO ## port_name, GPIO_PIN_ ## pin_number);

void Pin_set(char port, int pin, bool value);
void Pin_tog(char port, int pin);


class Pin {
    char port;
    int pin_number;

public:
    Pin_IRQ IRQ = Pin_IRQ();

public:
    Pin(char port_set = 'X', int pin_number_set = 0);

    bool operator==(const Pin& rhs);

    void Toggle();
    void Set(bool value);
    bool Read();
};




#endif // ifndef PIN_H
