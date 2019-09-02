#pragma once

#include "entry.hpp"

#include <string>
#include <vector>

using namespace std;

class Directory: public FS_entry
{
private:
    string name = "None";
protected:
    vector<FS_entry *> content;
public:
    Directory();
    Directory(string name);

    int Delete_entry(FS_entry &entry);
    int Add_entry(FS_entry &entry);

    const inline string Get_name(){return name;};
    const inline int Contains(){return content.size();};
};

