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
private:

public:
    virtual int Put(uint x, uint y) = 0;
    virtual int Clear(uint x, uint y) = 0;
};
