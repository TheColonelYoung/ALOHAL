/**
 * @file SSD1306.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 25.09.2019
 */

#pragma once

#include <cerrno>
#include <vector>
#include <map>

#include "graphical_display.hpp"
#include "i2c/i2c_device.hpp"

typedef unsigned int uint;

/**
 * @brief   OLED display with I2C capable controller
 *          Maximal length of transmitted data is 64 bytes (this not count address and first selection byte of communivation)
 *          If display is connected via other than parallel port, data cannot be read from it.
 *              Due to this must MCU contains content of whole display, this will tak around 1kB of RAM
 *          Memory architecture consists of pages. Page represent 8 lines of display. Page can have up to 128 columns.
 *              Column of page is type uint8_t. Represent 8 pixels beneath.
 *          Memory have adddress pointer to actual position, which consists of page and column pointer
 *              Smallest element which can be changed is column.
 *          In initialization is set horizontal mode, in this mode when column pointer overflows over 127,
 *              will increment page pointer. Example, P0,C127 -> P1,C0
 *          NOTE: setting addres other then 0,0 will restrict available area to square under this pixel
 */
class SSD1306: public Graphical_display, public I2C_device{
private:
    /**
     * @brief   Represents pixels of display, coresponds with memory organization of GDDRAM
     *          in display controller. Vector represents page, number in vector is column, records in map are pages.
     */
    map<uint8_t,vector<uint8_t>> bitmap;

    /**
     * @brief   Represent address pointer to GDDRAM of display
     *          First element is page, second is column
     */
    pair<uint8_t, uint8_t> address = pair<uint8_t, uint8_t>{0,0};

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
     *          This operation does not increments RAM address pointer of display.
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
