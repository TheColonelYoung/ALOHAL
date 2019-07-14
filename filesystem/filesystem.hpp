#pragma once

#include "directory.hpp"

#include <string>
#include <vector>

class Filesystem
{
private:
    Directory root;
public:
    Filesystem() =default;
    ~Filesystem();
};
