/**
 * @file device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.07.2019
 */
#pragma once

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L

#include <string>
#include <vector>
#include <memory>

#include "mcu/mcu.hpp"
#include "cli/cli.hpp"
#include "gpio/pin.hpp"
#include "device/component.hpp"

using namespace std;

class Device{
private:

public:
    MCU *mcu = new MCU();
    CLI *cli = nullptr;

    vector<shared_ptr<Component>> components;

    Device() = default;

    /**
     * @brief   Initialization of device
     *          Runs initialization of mcu
     */
    void Init();

    /**
     * @brief Creates and initialize CLI for device
     *
     * @param connection Uart connection which will be used for CLI
     * @return int
     */
    int Enable_CLI(UART *connection);

    /**
     * @brief Creates new enumerated name from generic component name
     *
     * @param original_name     Name of generic component, example: L6470
     * @return string           New generated name with enumeration, example: L6470_1
     */
    string New_component_name(string original_name);

    /**
     * @brief Add component to vector of known components
     *
     * @param new_component Component to add
     * @return uint         Number of components
     */
    uint Register_component(shared_ptr<Component> new_component);

};


