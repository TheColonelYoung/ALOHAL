/**
 * @file SSD1306.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 25.09.2019
 */

#pragma once

#include <cerrno>

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

    /**
     * @brief Set pixel determinated by coordinates to white(1)
     *
     * @param x     X coordinates
     * @param y     Y coordinates
     * @return int  Error code
     */
    virtual int Put(uint x, uint y) final override;

    /**
     * @brief Set pixel determinated by coordinates to black(0)
     *
     * @param x     X coordinates
     * @param y     Y coordinates
     * @return int  Error code
     */
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

    /**
     * @brief Set all pixel in image to zero(black)
     */
    void Clear_all();

    /**
     * @brief           Set RAM address pointer to selected column inside selected page
     *
     * @param page      Number of page (0-7)
     * @param column    Number of column of display (0-127)
     * @return int      Error code
     */
    int Set_address(uint8_t page, uint8_t column);

    /**
     * @brief   Write given data at display to the currently selected address
     *          This operation increments RAM address pointer of display
     *
     * @param content   Content of one column - 8 pixels
     * @return int      Error code
     */
    int Set_column_content(uint8_t content);

    /**
     * @brief   Read data from display GDDRAM, reading is performed from currently set address
     *          This operation increments RAM address pointer of display
     *
     * @return uint8_t      Content of column
     */
    uint8_t Read_column_content();

private:
    /**
     * @brief   Transmit command to OLED constroller, before command self is transmitted empty byte,
     *              which marks another byte as command byte
     *
     * @param cmd   Command which will be transmitted to OLED controller
     */
    void Send_command(uint8_t cmd);

};
