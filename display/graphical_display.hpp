/**
 * @file graphical_display.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 25.09.2019
 */

#pragma once

#include "display.hpp"

typedef unsigned int uint;
/**
 * @brief   Represent monochromatic graphic display. On display could be printed any type of graphic created from pixels.
 */
class Graphical_display: public Display
{
public:
    /**
     * @brief Construct a new Graphical_display object with selected resolution
     *
     * @param res_x Resolution in X axis
     * @param res_y Resolution in y axis
     */
    Graphical_display(uint res_x, uint res_y);

    /**
     * @brief Lights on pixel at given position
     *
     * @param x     Pixel posion on X axis
     * @param y     Pixel posion on Y axis
     * @return int  Error code
     */
    virtual int Put(uint x, uint y) = 0;

    /**
     * @brief Turns off pixel at given position
     *
     * @param x     Pixel posion on X axis
     * @param y     Pixel posion on Y axis
     * @return int  Error code
     */
    virtual int Clear(uint x, uint y) = 0;
};
