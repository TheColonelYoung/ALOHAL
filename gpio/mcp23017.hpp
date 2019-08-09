/**
 * @file mcp23017.cpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

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
    /**
     * Variables below are mirroring registers in MCP23017
     * This can save some transmission, for example when pin in state 1 is set to 1
     */
    uint16_t state;
    uint16_t direction;
    uint16_t pull_up;
    uint16_t irq_en;

public:
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

    enum IRQ_trigger{     // GPINTEN  DEFVAL  INTCON
        disabled     = 0, //    0       X       X
        on_change    = 1, //    1       X       0
        rising_edge  = 2, //    1       0       1
        falling_edge = 3  //    1       1       1
    }

    /**
     * @brief Constructors are inherited from I2C_device
     */
    using I2C_device::I2C_device;

    /**
     * @brief Initialize IC via I2C to default configuration
     *
     * Configuration of MCP23017 (register IOCON)- 0b01100000
     * BANK   = 0  Registers of ports are interleaved
     * MIRROR = 1  Interrupt notify pins are mergen into one
     * SEQOP  = 1  Address increment is disabled
     * DISSLW = 0  Slew rate control of SDA is disabled
     * HAEN   = 0  Only in SPI version (23S17)
     * ODR    = 0  Active output driver
     * INTPOL = 0  IRQ notify is active-low
     * NOTIMP = 0  Not used flag
     *
     * Polarity of input pin is same as logic at default IPOL
     */
    void Init();

    /**
     * @brief Set pin of expander to output state
     *
     * @param pin Number of pin 0-15, Port A 0-7, Port B 8-15
     * @param state New state of pin
     * @return int
     */
    int Set(uint8_t pin, bool state);

    int Read(uint8_t pin);

    int Direction(uint8_t pin, bool direction);

    int Pull_up(uint8_t pin, bool state)

    int IRQ_setup(uint8_t pin, IRQ_trigger trigger)

    bool IRQ_read(uint8_t pin);

    uint16_t IRQ_read();


};
