/**
 * @file component.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 30.08.2019
 */

#pragma once

#include <string>

typedef unsigned int uint;

#include "globals.hpp"
#include "device.hpp"
#include "filesystem/file.hpp"

using namespace std;

/**
 * @brief   Represents parts on PCB which is connected with mcu someway
 *          Used for any parts of device, for example some I2C memory or stepper motor drivers
 */
class Component {
private:
    /**
     * @brief Shared counter, from which anew components load their id
     */
    inline static size_t id_counter = 0;

protected:
    string name = "Unknown_component";

    /**
     * @brief Unique identifier across all components inside device
     */
    uint id;

public:
    /**
     * @brief   Construct a new Component object, with enumerated name and register it to device
     *          Enumerated name became nre component name
     *
     * @param name Original name for Component, example L6470
     */
    Component(string name);

    /**
     * @brief Variable Name gettor
     *
     * @return string Name of component
     */
    string Name() const { return name; };

    /**
     * @brief Variable ID gettor
     *
     * @return uint ID of component
     */
    uint Id() const { return id; };

protected:

    /**
     * @brief           Create virtual file in component folder
     *
     * @tparam class_T  Class of object which provide data for file
     * @tparam return_type_T    Return type of provider method
     * @param name      Filename in Filesystem
     * @param object    Pointer to object which provides data
     * @param method    Method of object to execute, must return double or string
     * @return true     File is created in component folder
     * @return false    Device filesystem is not available
     */
    template <typename class_T, typename return_type_T>
    bool Create_virtual_file(string name, class_T *object, return_type_T (class_T::*method)(void)){
        if (device()->Filesystem_available()) {
            File<class_T> *component_file = new File<class_T>(name, object, method);
            device()->fs->Add_entry("/components/" + Name() + "/" + name, component_file);
        } else {
            return false;
        }
        return true;
    }
};
