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

class Directory : public FS_entry
{
private:
    vector<FS_entry *> content;

public:
    Directory();

    /**
     * @brief Construct a new Directory object
     *
     * @param name Path to where create directory
     */
    Directory(string name);

    ~Directory();

    /**
     * @brief Add entry to directory content
     *
     * @param entry Pointer to entry which will be added to directory content
     * @return int  Number of entries inside directory
     */
    int Add_entry(FS_entry *entry);

    /**
     * @brief   Removed given entry from directory content
     *
     * @param entry Pointer to entry which will be removed from directory content
     * @return int  Number of entries inside directory
     */
    int Delete_entry(FS_entry *entry);

    /**
     * @brief   Count number of entries
     *
     * @return const int    Number of entries in Directory
     */
    const inline int Size() const { return content.size(); };

    /**
     * @brief Return entries, which are contained in Directory
     *
     * @return const vector<FS_entry *> Content of folder
     */
    const inline vector<FS_entry *> Contains() const { return content; };

    /**
     * @brief Returns pointer to directory defined by filename
     *
     * @param entry_name        File name to search
     * @return const FS_entry*  Pointer to entry with given name
     */
    FS_entry * Get_entry(const string entry_name) const;

    /**
     * @brief Check if entry with given name exists in folder
     *
     * @param filename  Filename to check
     * @return true     File exists
     * @return false    File not exists
     */
    const bool Exists(const string filename);

    /**
     * @brief Return names of all contained entries
     *
     * @return vector<string> Name of entries in directory
     */
    vector<string> Content_names();
};
