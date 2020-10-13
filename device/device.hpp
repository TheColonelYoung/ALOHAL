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
#include "events/planner/planner.hpp"

class Component;
class Application;
class Tool;

using namespace std;

class Device {
private:

public:
    inline static Device* instance = nullptr;

    MCU *mcu = new MCU();
    CLI *cli;

    Filesystem *fs;

    map<string, Application *> applications;

    map<string, Tool *> tools;

    vector<Component *> components;

    vector<Planner *> planners;

private:
    Device() = default;

public:
    static Device* Instance();
    /**
     * @brief   Initialization of device
     *          Runs initialization of mcu
     */
    void Init();

    /***************************************************
    *                      CLI
    * *************************************************/

    /**
     * @brief Creates and initialize CLI for device
     *
     * @param connection Serial line connection which will be used for CLI
     * @return int
     */
    int Enable_CLI(Serial_line *connection);

    /**
     * @brief Check if CLI for device exists
     *
     * @return true     CLI is available
     * @return false    CLI is not available
     */
    bool CLI_available();

    /***************************************************
    *                 FILESYSTEM
    * *************************************************/

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

    /***************************************************
    *                APPLICATIONS
    * *************************************************/

    /**
     * @brief Add application to map of known application
     *
     * @param new_component Pointer to application to add
     * @return string       Return code, 0 if all is OK,
     */
    int Register_application(Application *new_application);

    /***************************************************
    *                COMPONENTS
    * *************************************************/

    /**
     * @brief Add component to vector of known components
     *
     * @param new_component Pointer to component to add
     * @return string       New name of component
     */
    string Register_component(Component *new_component);

    /***************************************************
    *                TOOLS
    * *************************************************/

    /**
     * @brief Add tool to map of known components
     *
     * @param new_component Pointer to tool to add
     * @return string       New name of tool
     */
    int Register_tool(Tool *new_tool);

    /***************************************************
    *                 PLANNERS
    * *************************************************/

    /**
     * @brief Register planner
     *
     * @param planner   Pointer to planner
     * @return int      Number of registred planners
     */
    int Register_planner(Planner *planner);

    /**
     * @brief Remove planner from registred planners
     *
     * @param planner   Pointer to planner
     * @return int      Number of registred planners
     */
    int Unregister_planner(Planner *planner);

    /**
     * @brief Return planner designated by name
     *
     * @param name      Name of planner
     * @return Planner* Pointer to planner with given name
     */
    Planner * Get_planner(string name);

private:

    /**
     * @brief Creates new enumerated name from generic component name
     *
     * @param original_name     Name of generic component, example: L6470
     * @return string           New generated name with enumeration, example: L6470_1
     */
    string New_component_name(string original_name);
};
