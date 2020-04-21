/**
 * @file history.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 20.04.2020
 */

#pragma once

#include <vector>
#include <string>

#include "globals.hpp"

using namespace std;

/**
 * @brief   Stores lastly used command in CLI
 *          Can store only limited number of commands
 *          Works as circular buffer, dump oldest record when new is added
 *          Contain pointer to currently selected command, so commands can be listed
 */
class CLI_history
{
private:
    /**
     * @brief  Buffer for history of commands
     */
    vector<string> cmd_history;

    /**
     * @brief Maximal count of commands
     */
    int size;

    /**
     * @brief Index of newest record
     */
    int begin = 0;

    /**
     * @brief Index of oldest record
     */
    int end = 0;

    /**
     * @brief Index of currently selected record
     */
    int pointer = 0;

public:
    /**
     * @brief       Construct a new cli history object
     *
     * @param size  Size of history buffer
     */
    CLI_history(uint size);
    ~CLI_history() =default;

    /**
     * @brief Move down in history, from new -> old
     *
     * @return string   Currently selected command
     */
    string Up();

    /**
     * @brief Move down in history, from old -> new
     *
     * @return string   Currently selected command
     */
    string Down();

    /**
     * @brief Add new record to command history
     *
     * @param new_command   Text of new command
     * @return int          1 is command is same as newest command
     */
    int Update(string new_command);

    /**
     * @brief Reset pointer to most actual record
     *        Used at every new line in CLI
     */
    void Reset_pointer();
};

