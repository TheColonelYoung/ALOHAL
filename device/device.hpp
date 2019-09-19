/**
 * @file device.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 13.07.2019
 */
#pragma once

#if __cplusplus > 199711L
# define register // Deprecated in C++11.
#endif            // #if __cplusplus > 199711L

#include <string>
#include <vector>
#include <algorithm>

#include "mcu/mcu.hpp"
#include "cli/cli.hpp"
#include "filesystem/filesystem.hpp"
#include "gpio/pin.hpp"

class Component;

using namespace std;

class Device {
private:

public:
    inline static Device* instance = nullptr;

    MCU *mcu = new MCU();
    CLI *cli;

    Filesystem *fs;

    vector<Component *> components;

    vector<Component*> components;

private:
    Device() = default;

public:
    static Device* Instance();
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
     * @brief Check if CLI for device exists
     *
     * @return true     CLI is available
     * @return false    CLI is not available
     */
    bool CLI_available();

    /**
     * @brief   Creates and initialize Filesystem for device
     *          File system need to have enabled CLI
     *
     * @return int 0 if filesystem is running
     */
    int Enable_Filesystem();

    /**
     * @brief Check if device has filesystem
     *
     * @return true     filesystem is available
     * @return false    filesystem is not available
     */
    bool Filesystem_available();

    /**
     * @brief Add component to vector of known components
     *
     * @param new_component Pointer to component to add
     * @return string       New name of component
     */
    string Register_component(Component* new_component);

private:
    /**
     * @brief Creates new enumerated name from generic component name
     *
     * @param original_name     Name of generic component, example: L6470
     * @return string           New generated name with enumeration, example: L6470_1
     */
    string New_component_name(string original_name);
};

#include "device/component.hpp"
