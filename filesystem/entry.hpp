#pragma once

//#include "directory.hpp"

#include <string>

using namespace std;

class FS_entry{
protected:

    enum class Type{
        Undefined,
        Directory,
        File,
        Executable
    };

    Type type = Type::Undefined;

    //Directory *parent = nullptr;
    string name = "None";
public:
    FS_entry() =default;
    //FS_entry(string name, Directory &dir):name(name),parent(&dir){};

    ~FS_entry() =default;

    int Delete();
    string Get_name(){return name;};

    string Path(){return "/";};
};
