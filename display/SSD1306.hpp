/**
 * @file SSD1306.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 25.09.2019
 */

#pragma once

#include "graphical_display.hpp"
#include "i2c/i2c_device.hpp"

class SSD1306: public Graphical_display, public I2C_device{
private:

public:
    using I2C_device::I2C_device;

    virtual void On() final override;

    virtual void Off() final override;

    virtual int Put(uint x, uint y) final override;

    virtual int Clear(uint x, uint y) final override;

};
