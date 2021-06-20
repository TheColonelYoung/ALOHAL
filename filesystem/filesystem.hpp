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
#include "misc/text_transform.hpp"

#include <algorithm>
#include <cerrno>
#include <string>
#include <vector>
#include <functional>

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
    int Command_fg(vector<string> args);

    /**
     * @brief   Creates empty directory at given path
     *
     * @param name Path to folder, last part of path is directory name
     * @return int Error number
     */
    int Make_directory(string path);

    /**
     * @brief   Create file inside RAM
     *
     * @param name          File name
     * @param content       Content of file, can be empty
     * @return int          Error number
     */
    int Make_file(string path, string content = "");

    /**
     * @brief   Create virtual file inside filesystem
     *
     * @tparam class_T  Class of object which is pass to method
     * @param path      Location at which will be file created
     * @param object    Pointer to object which will be providingcontent of file
     * @param method    Method which will invocated on saved object, must return string and receive void as argument
     * @return int      Error number
     */
    template<typename class_T>
    int Make_file(string path, class_T *object, string (class_T::*method) (void)){
        string filename = path.substr(path.find_last_of("/") + 1);
        File<class_T> *new_file = new File<class_T>(filename, object, method);
        return Add_entry(path, new_file);
    }

    /**
     * @brief       Create virtual file inside filesystem
     *
     * @param path  Location at which will be file created
     * @param iw    Invocation wrapper with function(lambda) which supply content of file
     * @return int  Error number
     */
    int Make_file(string path, Invocation_wrapper<void, string, void> * iw){
        string filename = path.substr(path.find_last_of("/") + 1);
        File<string> *new_file = new File<string>(filename, iw);
        return Add_entry(path, new_file);
    }

    /**
     * @brief       Create virtual file inside filesystem
     *
     * @param path  Location at which will be file created
     * @param iw    Invocation wrapper with function(lambda) which supply content of file
     * @return int  Error number
     */
    int Make_file(string path, Invocation_wrapper<void, double, void> * iw){
        string filename = path.substr(path.find_last_of("/") + 1);
        File<string> *new_file = new File<string>(filename, iw);
        return Add_entry(path, new_file);
    }

    /**
     * @brief   Create executable inside filesystem
     *
     * @tparam class_T  Class of object which is pass to method
     * @param path      Location at which will be executable saved
     * @param object    Pointer to object which will be providing executable method
     * @param method    Method which will invocated on saved object, must return int and receive vector<string> as argument
     * @return int      Error number
     */
    template<typename class_T>
    int Make_executable(string path, class_T *object, int(class_T::*method)(vector<string>&)){
        auto iw = new Invocation_wrapper<class_T, int, vector<string> &>(object, method);
        return Make_executable(path, iw);
    }

    /**
     * @brief   Create executable inside filesystem
     *
     * @tparam class_T  Class of object which is pass to method
     * @param path      Location at which will be executable saved
     * @param object    Pointer to object which will be providing executable method
     * @param method    Method which will invocated on saved object, must return int and receive vector<string> as argument
     * @return int      Error number
     */

    template<typename class_T>
    int Make_executable(string path, Invocation_wrapper<class_T, int, vector<string> &> *iw){
        string filename = path.substr(path.find_last_of("/") + 1);
        Executable *new_exec = new Executable(filename, iw);
        return Add_entry(path, new_exec);
    }

    /**
     * @brief   Create executable inside filesystem
     *
     * @tparam class_T  Class of object which is pass to method
     * @param path      Location at which will be executable saved
     * @param object    Pointer to object which will be providing executable method
     * @param method    Method which will invocated on saved object, must return int and receive vector<string> as argument
     * @return int      Error number
     */
/*
    int Make_executable(string path, Invocation_wrapper_base<int, vector<string> &> *iw){
        string filename = path.substr(path.find_last_of("/") + 1);
        Executable *new_exec = new Executable(filename, iw);
        return Add_entry(path, new_exec);
    }*/

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
     * @brief   Determinate type of entry
     *
     * @param filename          Path to fs record
     * @return FS_entry::Type   Type of record at path
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

    /**
     * @brief Method called from CLI, which run executable from filesystem
     *
     * @param path  Path to executable
     * @param args  Arguments for executable
     * @return int  Return code of executable
     */
    int Execute(string &path, vector<string> args);

    /**
     * @brief Delete entry from filesystem
     *
     * @param entry Pointer to entry which will be deleted
     * @return int  Error number
     */
    int Delete(FS_entry *entry);

    /**
     * @brief Set current location in file system if exists
     *
     * @param path
     * @return int
     */
    int Set_location(string path);

    /**
     * @brief Return current location in filesystem
     *
     * @return Directory* Pointer to current directory
     */
    inline Directory *Current_location() const {return actual_position;};

    /**
     * @brief Return name of all entries in currently selected folder
     *
     * @return vector<string>   Names of entries
     */
    vector<string> Current_location_content() const {return Current_location()->Content_names();};

private:
    /**
     * @brief Check if input arguments of generic commands are valid
     *
     * @param args      Command arguments
     * @return string   Absolute path for first argument
     */
    string Command_check(vector<string> args) const;

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

};
