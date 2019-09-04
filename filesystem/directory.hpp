/**
 * @file directory.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "entry.hpp"

#include <string>
#include <vector>

using namespace std;

class Directory: public FS_entry
{
private:
    string name = "None";
    vector<FS_entry *> content;

public:
    Directory();
    Directory(const string name);

    int Delete_entry(FS_entry &entry);
    int Add_entry(FS_entry &entry);

    /**
     * @brief   Count number of entries
     *
     * @return const int    Number of entries in Directory
     */
    const inline int Size(){return content.size();};

    /**
     * @brief Return entries, which are contained in Directory
     *
     * @return const vector<FS_entry *> Content of folder
     */
    const inline vector<FS_entry *> Contains(){return content;};

    /**
     * @brief Returns pointer to directory defined by filename
     *
     * @param entry_name        File name to search
     * @return const FS_entry*  Pointer to entry with given name
     */
    FS_entry* Get_entry(const string entry_name) const;

    /**
     * @brief Check if entry with given name exists in folder
     *
     * @param filename  Filename to check
     * @return true     File exists
     * @return false    File not exists
     */
    const bool Exists(const string filename);

};

