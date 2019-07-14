#pragma once

#include "directory.hpp"

#include <string>

using namespace std;

class FS_entry{
protected:
    Directory *parent = nullptr;
    string name = "None";
public:
    FS_entry() =default;
    FS_entry(string name, Directory &dir):name(name),parent(&dir){};

    ~FS_entry();

    int Delete();
    string Get_name(){return name;};
};
