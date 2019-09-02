#pragma once

#include "directory.hpp"
#include "cli/cli.hpp"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

class Filesystem
{
private:


    shared_ptr<Directory> root { new Directory("/")};

    shared_ptr<Directory> actual_position;

    shared_ptr<CLI> cli;
public:
    Filesystem() =default;
    Filesystem(shared_ptr<CLI> cli);

    const int Command_ls(vector<string> args);
    int Command_cd(vector<string> args);
    const int Command_cat(vector<string> args);
    const int Command_pwd(vector<string> args);

    int Make_directory(string name);
    int Make_executable(string name);

    const vector<string> Create_entry_path(string filename);
    const FS_entry Get_entry(vector<string> path);

    const bool Entry_exists(string filename);

    int Delete(string name);
};
