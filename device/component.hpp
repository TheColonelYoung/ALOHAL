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
class Component{
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
    string Name() const;

protected:
    template<typename class_T>
    bool Create_virtual_file(string name, class_T* object, double(class_T::*method)(void)){
        if (device()->Filesystem_available()){
            File<class_T>* component_file = new File<class_T>(name, object, method);
            device()->fs->Add_entry("/components/" + Name() + "/" + name, component_file);
        } else {
            return false;
        }
        return true;
    }

    template<typename class_T>
    bool Create_virtual_file(string name, class_T* object, string(class_T::*method)(void)){
        if (device()->Filesystem_available()){
            File<class_T>* component_file = new File<class_T>(name, object, method);
            device()->fs->Add_entry("/components/" + Name() + "/" + name, component_file);
        } else {
            return false;
        }
        return true;
    }
};
