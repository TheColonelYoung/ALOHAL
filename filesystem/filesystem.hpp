/**
 * @file filesystem.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "cli/cli.hpp"
#include "directory.hpp"
#include "entry.hpp"
#include "executable.hpp"
#include "file.hpp"

#include <algorithm>
#include <cerrno>
#include <string>
#include <vector>

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
    int Command_cat(vector<string> args);
    int Command_pwd(vector<string> args);

    /**
     * @brief   Creates empty directory at given path
     *
     * @param name Path to folder, last part of path is directory name
     * @return int 0 if it was successful
     */
    int Make_directory(string path);

    /**
     * @brief   Create file inside RAM
     *
     * @param name          File name
     * @param content       Content of file, can be empty
     * @return int          0 if it was successful
     */
    int Make_file(string path, string content = "");

    /**
     * @brief   Create virtual file inside filesystem
     *
     * @tparam class_T  Class of object which is pass to method
     * @param path      Location at which will be file created
     * @param object    Pointer to object which will be providingcontent of file
     * @param method    Method which will invocated on saved object
     * @return int  0 if it was successful
     */
    template<typename class_T>
    int Make_file(string path, class_T *object, int (class_T::*method) (void)){
        return 0;
    }

    /**
     * @brief   Create executable inside filesystem
     *
     * @tparam class_T  Class of object which is pass to method
     * @param path      Location at which will be executable saved
     * @param object    Pointer to object which will be providing executable method
     * @param method    Method which will invocated on saved object
     * @return int  0 if it was successful
     */
    template<typename class_T>
    int Make_executable(string path, class_T *object, int(class_T::*method)(vector<string>)){
        return 0;
    }

    /**
     * @brief   Creates absolute path from given path
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

    /**
     * @brief Add entry into file system
     *
     * @param path      Path to new file, with filename
     * @param entry     Entry to add
     * @return int      Error number
     */
    int Add_entry(string path, FS_entry *entry);


    int Delete(FS_entry *entry);

private:
    string Command_check(vector<string> args) const;
};
