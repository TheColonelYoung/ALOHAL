/**
 * @file SSD1306.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 25.09.2019
 */

#pragma once

#include "graphical_display.hpp"
#include "i2c/i2c_device.hpp"

typedef unsigned int uint;

/**
 * @brief   OLED display with I2C capable controller
 */
class SSD1306: public Graphical_display, public I2C_device{
public:
    /**
     * @brief Constructor inherited from I2C device
     */
    using I2C_device::I2C_device;

    SSD1306(uint res_x, uint res_y, I2C_master master, unsigned char address);

    /**
     * @brief Enable display via command of display
     */
    virtual void On() final override;

    /**
     * @brief Disable display via command of display
     */
    virtual void Off() final override;

    /**
     * @brief   Perform initialization with setting up oscillator, multiplexer,
     *              Segment remaping, pre-charge period, disable inverting display, set charge pump
     */
    virtual void Init() final override;

    virtual int Put(uint x, uint y) final override;

    virtual int Clear(uint x, uint y) final override;

    /**
     * @brief Set contract of display via available command
     *
     * @param contrast  Constrast level in range 0-255
     * @return int      Error code
     */
    virtual int Set_contrast(uint8_t contrast) final override;

    /**
     * @brief Set all pixels of display to On state
     */
    void All_on();

    void Print();

    void Clear_all();

    int Set_address(uint8_t page, uint8_t column);

private:
    /**
     * @brief   Transmit command to OLED constroller, before command self is transmitted empty byte,
     *              which marks another byte as command byte
     *
     * @param cmd   Command which will be transmitted to OLED controller
     */
    void Send_command(uint8_t cmd);

};
