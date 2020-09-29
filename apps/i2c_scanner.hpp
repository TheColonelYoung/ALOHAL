/**
 * @file i2c_scanner.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 28.09.2020
 */

#pragma once

#include <string>
#include <vector>

#include "device/application.hpp"
#include "i2c/i2c_master.hpp"
#include "misc/text_transform.hpp"

using namespace std;

/**
 * @brief
 *
 */
class I2C_scanner: public Application
{
private:
    /**
     * @brief master of I2C bus
     */
    I2C_master *bus;

public:
    /**
     * @brief Construct a new i2c scanner object
     */
    I2C_scanner(I2C_master *bus);

    /**
     * @brief Do nothing
     *
     * @return int Always 0
     */
    virtual int Init() final override;

    /**
     * @brief   Print test text on cli if available
     *          Otherwise do nothing
     *
     * @param args  Anything
     * @return int  0 if CLi is available, 1 otherwise
     */
    virtual int Run(vector<string> &args) final override;

    /**
     * @brief Currently do nothing
     *
     * @param input     Ignored
     * @return int      Only 0
     */
    virtual int Input_load(string input) final override;
};
