#include "pin.hpp"

Pin::Pin(char port_set, int pin_number_set)
{
	port       = port_set;
	pin_number = pin_number_set;
}

void Pin::Toggle()
{
	HAL_GPIO_TogglePin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin_number);
}

void Pin::Set(bool value)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin_number, (GPIO_PinState) value);
}

// Base port adress:1207959552
void Pin_set(char port, int pin, bool value)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin, (GPIO_PinState) value);
} /* Pin_set */

void Pin_tog(char port, int pin)
{
	HAL_GPIO_TogglePin((GPIO_TypeDef *) (PORT_START_ADRESS + ((((int) port) - ASCII_BASE) * PORT_SIZE )), (uint16_t) 1 << pin);
}
