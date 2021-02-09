/**
 * @file executable.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "entry.hpp"
#include "misc/invocation_wrapper.hpp"

#include <string>
#include <vector>

using namespace std;

/**
 * @brief   Represent executable entry inside filesystem
 *          Contains object and his method, which will be invocated when this file is executed
 *          Method must be returning inteager and argument must be vector of strings
 *
 * @tparam class_T   Class of object on which will be method called
 */
//template <typename class_T>
class Executable : public FS_entry
{
private:

    /**
     * @brief Wrapper which hold object and method
     */
    Invocation_wrapper_base<int, vector<string> &> *executable;

public:
    Executable(){ this->type = Type::Executable; };

    ~Executable(){
        if (executable) {
            delete executable;
        }
    }

    /**
     * @brief Construct a new Executable object which hold pointer to object and method
     *
     * @param name      Filename of executable
     * @param object    Pointer to object which will be stored
     * @param method    pointer to method which will be Invocated at execution time
     */
    /*
    Executable(string name, class_T *object, int (class_T::*method) (vector<string> &)) :
        executable(new Invocation_wrapper<class_T, int, vector<string> &>(object, method)){
        this->type = Type::Executable;
        this->name = name;
    }*/

    /**
     * @brief Construct a new Executable object which holds invocation wrapper
     *
     * @param name
     * @param executable    Invocation wrapper to execute, must be allocated
     *                      Deallocated when executable is deleted
     */
    Executable(string name, Invocation_wrapper_base<int, vector<string> &> *executable) :
        executable(executable){
        this->type = Type::Executable;
        this->name = name;
    }

    /**
     * @brief Invoke method stored inside executable
     *
     * @param args  Arguments for encapsulated method
     * @return int  Return value of encapsulated method
     */
    int Run(vector<string> &args) const override {
        return executable->Invoke(args);
    }
};
