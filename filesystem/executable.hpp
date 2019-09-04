/**
 * @file executable.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "entry.hpp"

#include <string>

using namespace std;

class Executable: public FS_entry
{
private:


public:
    Executable();
    ~Executable();

    int Run(vector<string> args);
};
