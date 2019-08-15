/**
 * @file mcp23017.cpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 *
 * // TODO this IC can be optimazed for shorter transfer in case of one pin methods
 *         only one uint8_t needs to be transferred, other is unchanged
 */

#pragma once

#include <vector>

#include "i2c/i2c_device.hpp"

/**
 * @brief GPIO expander driven by MCU via I2C bus
 * Contains 16 I/O configurable pins which are interrupt capable
 * Even if IC has two ports, which every has 8 pins, this driver use 16 pin addressing
 * Number of pins are 0-15, Port A 0-7, Port B 8-15
 * Interrupt is configured via only one trace, so both port are are signaled together
 */
class MCP23017: public I2C_device
{
private:
    enum IRQ_trigger{     // GPINTEN  DEFVAL  INTCON
        disabled     = 0, //    0       X       X
        on_change    = 1, //    1       X       0
        rising_edge  = 2, //    1       0       1
        falling_edge = 3  //    1       1       1
    };

    /**
     * Variables below are mirroring registers in MCP23017
     * This can save some transmission, for example when pin in state 1 is set to 1
     * Number of pins is from right(0) to left(15)
     * Example: B7, B6, ... A2, A1, A0
     */
    uint16_t level      = 0x0000;     // logic level of output pin, 0-out, 1-in
    uint16_t direction  = 0xffff;     // at default is all set as input
    uint16_t pull_up    = 0x0000;     // at default is pull_up disabled

    // TODO IRQ trigger, need to be defined in main IRQ file
    vector<IRQ_trigger> irq_trigger();

    enum REG{
        IODIR   = 0x00,  // IO Dirrection Input/Outpus
        IPOL    = 0x02,  // Input polarity, if logic of input is same or inverted
        GPINTEN = 0x04,  // Interrupt-On-Change enable
        DEFVAL  = 0x06,  // Default interrupt comparison value,  Refer to INTCON
        INTCON  = 0x08,  // Interrupt compare value, 1 = compare against DEFVAL value, 0 = compare against the previous value
        IOCON   = 0x0A,  // IC setting, bank/mirror/seqop/disslw/haen/odr/intpol/notimp
        GPPU    = 0x0C,  // Pullup
        INTF    = 0x0E,  // Interrupt Flag, 1 = pin caused interrupt, 0 = no interrupt
        INTCAP  = 0x10,  // Interrupt capture value of GPIO at time of last interrupt
        GPIO    = 0x12,  // GPIO
        OLAT    = 0x14   // Output Latch
    };

    enum PORT{
        A = 0x00,
        B = 0x01,
    };

public:

    /**
     * @brief Constructors are inherited from I2C_device
     */
    using I2C_device::I2C_device;

    /**
     * @brief Initialize IC via I2C to default configuration
     * Configuration of IC is as below
     * Pins are set as input without pull_up resistor
     *
     * @return int Status of transmission
     *
     * Configuration of MCP23017 (register IOCON)- 0b01000000
     * BANK   = 0  Registers of ports are interleaved
     * MIRROR = 1  Interrupt notify pins are mergen into one
     * SEQOP  = 0  Address increment is enabled
     * DISSLW = 0  Slew rate control of SDA is disabled
     * HAEN   = 0  Only in SPI version (23S17)
     * ODR    = 0  Active output driver
     * INTPOL = 0  IRQ notify is active-low
     * NOTIMP = 0  Not used flag
     *
     * Polarity of input pin is same as logic at default IPOL
     */
    int Init();

    /**
     * @brief Set pin of expander to output state
     *
     * @param pin   Number of pin 0-15, Port A 0-7, Port B 8-15
     * @param state New state of pin
     * @return int  Status of transmission, -1 if pin is set as input
     */
    int Set(uint8_t pin, bool state);

    /**
     * @brief Set all pins at once
     *
     * @param port  New state for all pins [PORTA, PORTB]
     * @return int  Status of transmission
     */
    int Set(uint16_t port);

    /**
     * @brief Read input value from pins (all even set as output)
     *
     * @param pin  Number of pin 0-15
     * @return int Value on pin, -1 if pin is set as output
     */
    uint8_t Read(uint8_t pin);

    /**
     * @brief Read value from all pins
     * Even output pins are read
     *
     * @return uint16_t Value on pins
     */
    uint16_t Read();

    /**
     * @brief Toggle output pin value if pin is set as output
     *
     * @param pin_number Number of pin 0-15
     * @return uint8_t Status, -1 is pin is set as input, otherwise status of transmition
     */
    uint8_t Toggle(uint8_t pin_number);

    /**
     * @brief Setup pin  as output or input
     *
     * @param pin        Number of pin to setup
     * @param direction  true -> input, false -> output
     * @return int       Status of transmission
     */
    int Direction(uint8_t pin, bool new_direction);

    /**
     * @brief Setup direction of all pins
     *
     * @param port  Direction settings for all pins
     * @return int  Status of transmission
     */
    int Direction(uint16_t port);

    /**
     * @brief Set pull-up resistor of given pin
     * Ignored if pin is setup as output (but transmission is made)
     * Pull-up has value of 100k
     *
     * @param pin    Pin number 0-15
     * @param state  true -> pull-up enabled, false -> disabled
     * @return int   Status of transmission
     */
    int Pull_up(uint8_t pin, bool state);

    /**
     * @brief Set pull-up resistors of all pins
     *
     * @param port  Pull-up settings for pins
     * @return int  Status of transmission
     */
    int Pull_up(uint16_t port);

    /**
     * @brief Setup trigger of IRQ on pin or disable IRQ
     *
     * @param pin      Number of pin which is configured
     * @param trigger  Type of trigger
     * @return int     Status of transmission
     */
    int IRQ_setup(uint8_t pin, IRQ_trigger trigger);

    /**
     * @brief Read IRQ status of given pin
     * This operation erase flag in memory of IC
     * This method is used only part part of IRQ subsystem of this component
     *
     * @param pin   Number of pin for reading 0-15
     * @return int  Status of transmission
     */
    int IRQ_read(uint8_t pin);

    /**
     * @brief Read IRQ status of all pins at once
     * This method should be used preferably agains reading only one pins
     * Because during reading onlyone pin, IRQ of another pin can be lost
     *
     * @return uint16_t IRQ status
     */
    uint16_t IRQ_read();

};

