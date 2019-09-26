/**
 * @file display.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 25.09.2019
 */

#pragma once

#include <cstdint>

typedef unsigned int uint;

/**
 * @brief   Generic display, could be graphical or character display
 *          Resolution means pixel size or characters count on line and number of lines
 *          Origin point of display is in left top corner of display
 */
class Display{
public:
    /**
     * @brief Construct a new Display object with selected resolution
     *
     * @param res_x Resolution in X axis
     * @param res_y Resolution in y axis
     */
    Display(uint res_x, uint res_y);

    /**
     * @brief   Set display On, this can refer to backlight or enabling comunication
     *          Initialization Should be called before this command, but initialization should include this command
     *
     */
    virtual void On() = 0;

    /**
     * @brief   Disable display (backlight or whole power supply for display)
     */
    virtual void Off() = 0;

    /**
     * @brief   Initialization of display, after initialization display should be able to receive
     *              any other commands
     *          After initialization is display set to off state
     */
    virtual void Init() = 0;

    /**
     * @brief   Set display to selected contract
     *          Constract is set in range 0-255
     *
     * @return int Error number
     */
    virtual int Set_contrast(uint8_t) = 0;

protected:
    /**
     * @brief Resolution in X axis
     */
    uint resolution_x;

    /**
     * @brief Resolution in X axis
     */
    uint resolution_y;
};
