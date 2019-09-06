/**
 * @file filesystem.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "entry.hpp"
#include "directory.hpp"
#include "cli/cli.hpp"

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Filesystem
{
private:
    Directory* root = new Directory("/");

    Directory* actual_position = root;

    CLI* cli;
public:
    Filesystem() =default;
    Filesystem(CLI* cli);

    int Command_ls(vector<string> args);
    int Command_cd(vector<string> args);
    int Command_cat(vector<string> args) const;
    int Command_pwd(vector<string> args) const;

    int Make_directory(string name);
    int Make_executable(string name);

    /**
     * @brief Creates absolute path from given path
     *
     * @param path          Any type of path (absolute remain unchanged)
     * @return const string Absolute path to entry
     */
    string Absolute_path(string path) const;

    /**
     * @brief Breaks path in string to path consists of folder names (last is file name)
     *
     * @param filename      Path to file as string (absolute or relative)
     * @return const vector<string>    Names oif folders from root
     */
    vector<string> Create_entry_path(string filename) const;

    /**
     * @brief return FS_entry defined by relative or absolute path
     *
     * @param filename          Name of file relative or absolute
     * @return const FS_entry   Pointer to FS_entry defined by filename, if not exist, nullptr is returned
     */
    FS_entry* Get_entry(string filename) const;

    /**
     * @brief return FS_entry defined by folder names from root
     *
     * @param path              Names of folders from root to file
     * @return const FS_entry   Pointer to FS_entry defined by path, if not exist, nullptr is returned
     */
    FS_entry* Get_entry(vector<string> path) const;

    /**
     * @brief Test if entry with given name exists
     *
     * @param filename  Absolute or relative path to filein form of string
     * @return true     Entry exist in filesystem and can be accessed
     * @return false    Entry with this name does not exists
     */
    bool Entry_exists(string filename) const;

    /**
     * @brief Test if entry exists on given path
     *
     * @param filename  Names of folders from root
     * @return true     Entry exist in filesystem and can be accessed
     * @return false    Entry with this name does not exists
     */
    bool Entry_exists(vector<string> path) const;

    /**
     * @brief Test if entry exists on given path
     *
     * @param filename  Names of folders from root
     * @return true     Entry exist in filesystem and can be accessed
     * @return false    Entry with this name does not exists
     */
    FS_entry::Type Entry_type(string filename) const;

    int Delete(FS_entry entry);

private:
    string Command_check(vector<string> args) const;
};
