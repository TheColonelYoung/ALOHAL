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
#include <cerrno>

#include "i2c/i2c_device.hpp"
#include "gpio/pin_irq.hpp"
#include "gpio/pin.hpp"
#include "irq/irq_multi_handler.hpp"

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
     * Number of pins is from right(0) to left(15)
     * Example: B7, B6, ... A2, A1, A0
     * A0 - 0   A4 - 4      B0 - 8    B4 - 12
     * A1 - 1   A5 - 5      B1 - 9    B5 - 13
     * A2 - 2   A6 - 6      B2 - 10   B6 - 14
     * A3 - 3   A7 - 7      B3 - 11   B7 - 15
     */
    uint16_t level      = 0x0000;     // logic level of output pin, 0-out, 1-in
    uint16_t direction  = 0xffff;     // at default is all set as input
    uint16_t pull_up    = 0x0000;     // at default is pull_up disabled

    /**
     * @brief   Vector represents IRq trigger for all pins
     *          Below is register setting which coresponds with generic IRQ trigger type
     *               GPINTEN  DEFVAL  INTCON
     *  Disabled         0       X       X
     *  On change        1       X       0
     *  Rising edge      1       0       1
     *  Falling edge     1       1       1
     */
    vector<Pin_IRQ::Trigger> irq_triggers{vector<Pin_IRQ::Trigger>(16, Pin_IRQ::Trigger::Disabled)};

    /**
     * @brief   Pin on which is MCP23017 reposting pending interrupt must be configured to falling edge
     *          Due to mirroring of IRQ ports is only one pin needed for both ports
     */
    Pin *IRQ_input = nullptr;

    IRQ_multi_handler<int> IRQ;

    enum REG{
        IODIR   = 0x00,  // IO Dirrection 1-Input/0-Outpus
        IPOL    = 0x02,  // Input polarity, if logic of input is same or inverted
        GPINTEN = 0x04,  // Interrupt-On-Change enable
        DEFVAL  = 0x06,  // Default interrupt comparison value,  Refer to INTCON
        INTCON  = 0x08,  // Interrupt compare value, 1 = compare against DEFVAL value, 0 = compare against the previous value
        IOCON   = 0x0A,  // IC setting, bank/mirror/seqop/disslw/haen/odr/intpol/notimp
        GPPU    = 0x0C,  // Pullup 1-Enabled, 0 - Disabled
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
     * @brief   Set all pins at once
     *          Everytime when pin is set must performed reading and then writing
     *              to back to chip to ensure that no value was rewrite
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
     * @brief   Enable IRQ subsystem of MCP23017
     *          After enabling of IRQ all pins have diabled IRQ are must be enabled
     *              separately for each pin viamethod IRQ_setup
     *          Reporting pin must be setup as interrupt capable and set for falling edge
     *
     * @param IRQ_input     Pin on which MCP23017 reports pending IRQ
     * @return int          Error code
     */
    int IQR_Enable(Pin *IRQ_input);

    /**
     * @brief Setup trigger of IRQ on pin or disable IRQ
     *
     * @param pin      Number of pin which is configured
     * @param trigger  Type of trigger
     * @return int     Error code
     */
    int IRQ_setup(uint8_t pin, Pin_IRQ::Trigger trigger);

    /**
     * @brief   This method is called when IRQ occurs on IRQ input pin
     *          This means that MCP23017 is reporting IRQ on some pin
     *          During this method is determinated, which pin triggered IRQ
     *          Reporting pin must be set to falling edge IRQ
     */
    void IRQ_event();

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

    /**
     * @brief Read whole memory of expander
     *
     * @return vector<uint8_t> Whole memory region
     */
    vector<uint8_t> Memory_dump();

private:
    /**
     * @brief   Transmit 2 bytes to MCP23017, whole transmittion consists of 3 bytes
     *          First byte is target memory address, another two are data
     *
     * @param target_register   Address for writing (must add register address of MCP23017)
     * @param data              Data to write
     * @return int              Transmition status
     */
    int Transmit_16b(REG target_register, uint16_t data);

};

