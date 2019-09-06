/**
 * @file entry.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include <string>

using namespace std;

class FS_entry{
public:

    enum class Type{
        Undefined,
        Directory,
        File,
        Executable
    };

protected:
    Type type = Type::Undefined;

    string name = "None";

    /**
     * @brief Pointer to directory in which ius this entry located
     */
    FS_entry *parent = nullptr;

public:
    FS_entry() = default;

    FS_entry(string name);

    int Delete();

    /**
     * @brief Name of actual entry, this is not a path to entry
     *
     * @return string Name of entry
     */
    const inline string Name(){return name;};

    /**
     * @brief   Returns path from root to actual entry in form of string
     *          Path is constructed via recursion, so beware many recursion directories
     *
     * @return string Path from root
     */
    string Path() const;

    /**
     * @brief Return type of entry
     *
     * @return Type Entry type
     */
    const inline Type Type_of(){return type;};

    /**
     * @brief New parent can be set only when, is uninitialized (nullptr)
     *
     * @return true     New parent is set
     * @return false    Parent cannot be set, allready exists
     */
    bool Set_parent(FS_entry* entry);

    /**
     * @brief New parent can be set only when, is uninitialized (nullptr)
     *
     * @return true     New parent is set
     * @return false    Parent cannot be set, allready exists
     */
    inline FS_entry* Parent(){return parent;};
};
