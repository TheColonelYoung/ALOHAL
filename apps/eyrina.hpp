/**
 * @file eyrina.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 19.04.2020
 */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "device/application.hpp"
#include "globals.hpp"
#include "device/device.hpp"
#include "construction/motion_axis.hpp"

using namespace std;

/**
 * @brief   Application named Eyrina, is used for control of Eyrina device v4
 *          Accepts G-code commands and perform them
 *          Also initialize whole device and components
 */
class Eyrina : public Application
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
        int          (Eyrina::*method)(map<char, double>, vector<char> );
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
        { "G1",  { &Eyrina::G_code_G1,  { 'X', 'Y', 'Z', 'T', 'R', 'F' }, {   } } },
        { "G0",  { &Eyrina::G_code_G0,  { 'X', 'Y', 'Z', 'T', 'R', 'F' }, {   } } },
        { "G28", { &Eyrina::G_code_G28, {   }, { 'X','Y', 'Z', 'T', 'R', 'F' } } },
        { "E0",  { &Eyrina::G_code_E0,  {   }, { 'X','Y', 'Z', 'T', 'R', 'F' } } },
        { "E1",  { &Eyrina::G_code_E1,  {   }, { 'X','Y', 'Z', 'T', 'R', 'F' } } },
    };

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
    virtual int Run(vector<string> args) final override;

private:
    /**
     * @brief   Parse input command into g-code command, check if parameters are valid and pass them to command method
     *
     * @param gcode Separated ordered string which are parts of command: [Name] [Params + Value]* [Flags]
     * @return int  Return value of gcode if gcode is accepted, otherwise,
     *                  -1 if gcode with this name does not exists,
     *                  -2 if gcode with this name does not have given params
     *                  -3 if gcode with this name does not have given flags
     */
    int Parse(vector<string> gcode);

    /**
     * @brief Move axis to given position
     *
     * @param params    Names and positions of axis, which will be shifted, position is absolute
     * @param flags     Always empty, use only params
     * @return int      Number of axis which was been really moved (not counting axis, which is not enabled or etc.)
     */
    int G_code_G0(map<char, double> params, vector<char> flags);

    /**
     * @brief Move axis by given distance
     *
     * @param params    Names and distances of axis, which will be shifted, distances are relative to actual position
     * @param flags     Always empty, use only params
     * @return int      Number of axis which was been really moved (not counting axis, which is not enabled or etc.)
     */
    int G_code_G1(map<char, double> params, vector<char> flags);

    /**
     * @brief   Home given axis or all if no flag is given
     *
     * @param params    Always empty, use only flags
     * @param flags     Names of axis, which will be homed, if empty all axis will be homing
     * @return int      Number of axis which are homing
     */
    int G_code_G28(map<char, double> params, vector<char> flags);

    /**
     * @brief   Perform emergency hard stop at given or all axis
     *
     * @param params    Always empty, use only flags
     * @param flags     Names of axis, which will be stopped, if empty all axis will stop
     * @return int      Number of axis which was been hard stopped
     */
    int G_code_E0(map<char, double> params, vector<char> flags);

    /**
     * @brief   Perform soft stop at given or all axis
     *
     * @param params    Always empty, use only flags
     * @param flags     Names of axis, which will be stopped, if empty all axis will stop
     * @return int      Number of axis which was been soft stopped
     */
    int G_code_E1(map<char, double> params, vector<char> flags);
};
