/**
 * @file file.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "entry.hpp"

#include <string>

using namespace std;

class File: public FS_entry
{
private:
    string content;
public:
    File();
    ~File();

    inline void Set_content(string new_content){content.assign(new_content);};
    const inline string Get_content(){return content;};
    const inline long Size(){return content.length();};
};
