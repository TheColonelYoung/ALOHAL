#pragma once

#include "entry.hpp"

#include <string>

using namespace std;

class Executable: public FS_entry
{
private:
    /* data */
public:
    Executable() = default;
    ~Executable();
};
