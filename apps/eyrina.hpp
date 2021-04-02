/**
 * @file eyrina.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 19.04.2020
 */

#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>

#include "modifiers/loggable.hpp"
#include "device/application.hpp"
#include "construction/motion_axis.hpp"
#include "gpio/mcp23017/mcp23017.hpp"
#include "gpio/mcp23017/mcp23017_pin.hpp"
#include "motor/L6470.hpp"
#include "display/SSD1306.hpp"
#include "led/NCL30160.hpp"

using namespace std;

/**
 * @brief   Application named Eyrina, is used for control of Eyrina device v4
 *          Accepts G-code commands and perform them
 *          Also initialize whole device and components
 */
class Eyrina : public Application, public Loggable
{
private:

    /**
     * @brief       Structure which describes allowed parameters and flags of g-code command
     *
     * @var method  Pointer to method which is performing command, as argument must receive map<char, double>
     *                  and vector<char>
     * @var params  Allowed characters as parameters, parameter is further associated with numerical value
     * @var flags   Allowed characters as flags, is or isn't used
     */
    struct gcode_settings {
        int          (Eyrina::*method)(map<char, double>&, vector<char> &);
        vector<char> params;
        vector<char> flags;
    };

    /**
     * @brief       Connects G-code name with method which performs if and also define parameters and flags
     *
     * @member key      String which is name of command
     * @member value    Structure gcode_settings, which defines the command
     */
    const map<string, gcode_settings> g_code_commands = {
        { "G0",  { &Eyrina::G_code_G0,  { 'X', 'Y', 'Z', 'T', 'R', 'F' }, {   } } },
        { "G1",  { &Eyrina::G_code_G1,  { 'X', 'Y', 'Z', 'T', 'R', 'F' }, {   } } },
        { "G28", { &Eyrina::G_code_G28, {   }, { 'X','Y', 'Z', 'T', 'R', 'F' } } },
        { "M0",  { &Eyrina::G_code_M0,  {   }, { 'X','Y', 'Z', 'T', 'R', 'F' } } },
        { "M10",  { &Eyrina::G_code_M10,  {   }, { 'X','Y', 'Z', 'T', 'R', 'F' } } },
        { "E1",  { &Eyrina::G_code_E1,  {   }, { 'C','I'} } },
        { "E2",  { &Eyrina::G_code_E2,  {   }, { } } },
        { "E3",  { &Eyrina::G_code_E3,  {   }, { 'X','Y' } } },
        { "R0",  { &Eyrina::G_code_R0,  {   }, { 'M' } } },
        { "R10",  { &Eyrina::G_code_R10,  {   }, {   } } },
    };

    /**
     * @brief Pointers to all stepper drivers on board
     */
    vector<L6470 *> stepper_drivers;

    /**
     * @brief  Axis which can b controlled from application
     */
    map<char, Motion_axis*> axis;

    /**
     * @brief Queue for g-code commands
     */
    queue<string> command_buffer;

    /**
     * @brief Block queue after invalid command, unblock with R10
     */
    bool blocked_queue = false;

    /**
     * @brief OLED display from which target pointer is projected
     */
    SSD1306 *display = nullptr;

    /**
     * @brief Reflect status of OLED display, false means OFF
     */
    bool display_status = false;

    /**
     * @brief LED channels for retina illumination
     */
    vector<NCL30160 *> led_channels {nullptr, nullptr, nullptr};

public:
    /**
     * @brief Construct a new Eyrina application object inside device structure
     */
    Eyrina();
    ~Eyrina() = default;

    /**
     * @brief Initialization of all components: motion axis
     *
     * @return int 0 is all is OK, otherwise errno
     */
    virtual int Init() final override;

    /**
     * @brief   Parse and execute given gcode
     *
     * @param args  G-Code
     * @return int  0 is all is OK, otherwise errno
     */
    virtual int Run(vector<string> &args) final override;

    /**
     * @brief Add given command to buffer
     *
     * @param input     New command from user
     * @return int      Number of commands in buffer
     */
    virtual int Input_load(string input) final override;

    /**
     * @brief Add new axis to list of available axis
     *
     * @param axis_name Identifier of axis
     * @param new_axis  Pointer to new axis (type Motion_axis)
     * @return int      Number of registered axis
     */
    int Add_axis(char axis_name, Motion_axis *new_axis);
    
    /**
     * @brief Pop first command from buffer and process it
     * 
     * @return int  Number of commands in buffer
     */
    int Load_command();

private:
    
    /**
     * @brief Initialize all motion axis and their motors
     */
    void Init_motors();

    /**
     * @brief Initialize lights, current sources for LEDs and target point
     */
    void Init_light();

    /**
     * @brief   Parse input command into g-code command, check if parameters are valid and pass them to command method
     *          Controls syntax of command
     *
     * @param gcode Separated ordered string which are parts of command: [Name] [Params + Value]* [Flags]
     * @return int  Same as Validation, is only passing return value or add own
     *                  -4 if format of gcode command tag in invalid
     *                  -5 if format of parameter or flag is invalid
     */
    int Parse(vector<string> &gcode);

    /**
     * @brief   Check if given command is valid, so if correspods to table g_code_commands
     *          Controls semantic of code
     *
     * @param params        Parameters of command
     * @param flags         Flags of command
     * @param g_code_method Pointer to which is saved corrent g code setting struct containing g code method, returned to Parser
     * @return int      Return value of gcode if gcode is accepted, otherwise,
     *                  -1 if gcode with this name does not exists,
     *                  -2 if gcode with this name does not have given params
     *                  -3 if gcode with this name does not have given flags
     */
    int Validation(string &command, map<char, double> &params, vector<char> &flags, const gcode_settings *&g_code_method);

    /**
     * @brief Move axis to given position, absolute
     *
     * @param params    Names and positions of axis, which will be shifted, position is absolute
     * @param flags     Always empty, use only params
     * @return int      Number of axis which was been really moved (not counting axis, which is not enabled or etc.)
     */
    int G_code_G0(map<char, double> &params, vector<char> &flags);

    /**
     * @brief Move axis by given distance, relative
     *
     * @param params    Names and distances of axis, which will be shifted, distances are relative to actual position
     * @param flags     Always empty, use only params
     * @return int      Number of axis which was been really moved (not counting axis, which is not enabled or etc.)
     */
    int G_code_G1(map<char, double> &params, vector<char> &flags);

    /**
     * @brief   Home given axis or all if no flag is given
     *
     * @param params    Always empty, use only flags
     * @param flags     Names of axis, which will be homed, if empty all axis will be homing
     * @return int      Number of axis which are homing
     */
    int G_code_G28(map<char, double> &params, vector<char> &flags);

    /**
     * @brief   Perform soft stop at given or all axis,
     *          After stop motors are still engaged
     *
     * @param params    Always empty, use only flags
     * @param flags     Names of axis, which will be stopped, if empty all axis will stop
     * @return int      Number of axis which was been hard stopped
     */
    int G_code_M0(map<char, double> &params, vector<char> &flags);

    /**
     * @brief   Disable motors, possible danger of lost position accuracy
     *
     * @param params    Always empty, use only flags
     * @param flags     Names of axis, which on will will be motors disabled
     * @return int      Number of axis which was been soft stopped
     */
    int G_code_M10(map<char, double> &params, vector<char> &flags);
    
    /**
     * @brief   Set intensity of light channels
     * 
     * @param params    C - index of output light channel
     *                  I - Intensity 0.0 - 100.0 %
     * @param flags     Always empty
     * @return int      0 if successfull, or current channel error code
     */
    int G_code_E1(map<char, double> &params, vector<char> &flags);
    
    /**
     * @brief   Enable or disable OLED display, default index of target is [3,3]
     * 
     * @param params    Always empty
     * @param flags     Always empty
     * @return int      0 if successfull
     */
    int G_code_E2(map<char, double> &params, vector<char> &flags);
    
    /**
     * @brief   Set position of target point
     * 
     * @param params    X - horizontal index of position
     *                  Y - vertical index of position
     * @param flags     Always empty
     * @return int      Number of position which is set
     */
    int G_code_E3(map<char, double> &params, vector<char> &flags);
    
    /**
     * @brief   Perform emergency stop, without deceleration
     * 
     * @param params    Always empty
     * @param flags     M - if set only motor are disabled otherwise also light and target point is disabled
     * @return int      Number of disabled axis and light channels
     */
    int G_code_R0(map<char, double> &params, vector<char> &flags);
    
    /**
     * @brief Unblock queue of commands
     * 
     * @param params    Always empty
     * @param flags     Always empty
     * @return int      Number of commands in queue
     */
    int G_code_R10(map<char, double> &params, vector<char> &flags);
};
