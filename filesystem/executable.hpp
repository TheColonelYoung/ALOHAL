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
