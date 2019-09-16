/**
 * @file sensor.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 15.09.2019
 */

#pragma once

#include "device/component.hpp"
#include "misc/invocation_wrapper.hpp"

#include <string>
#include <vector>
#include <map>
#include <optional>

using namespace std;

/**
 * @brief   Serve as generic class, from which will be derivated all other type of sensors
 *          Provide universal interface, from which can be determined which quantities can sensor measure
 *          Also quantities which return type is numberical or text can be directly measured from this interface
 */
class Sensor : public Component{
private:

    /**
     * @brief   List of quantities, which can be measured by sensor, and their return type is double
     *          This type is used for example for: voltage sensor, temperature sensor, etc.
     */
    map<string, Invocation_wrapper_base<double, void> *> numerical_quantity;

    /**
     * @brief   List of quantities, which can be measured by sensor, and their return type is double
     *           This type is used for example for: status sensors
     */
    map<string, Invocation_wrapper_base<string, void> *> text_quantity;

    /**
     * @brief   List of quantities, which can be measured by sensor, but their return type is different then text or number (probably some structure)
     *          This type of quantities can be only measured by using specified methods, not via generic sensor interface
     *          This type is used for example for: GPS
     */
    vector<string> other_quantity;

public:
    /**
     * @brief Construct a new Sensor object using default sensor name = "Unknown_sensor"
     */
    Sensor();

    /**
     * @brief Construct a new Sensor object using given name
     *
     * @param name name of sensor
     */
    Sensor(string name);

    /**
     * @brief Create list of all available measurable quantities from numerical_quantity, text_quantity and other_quantity
     *
     * @return vector<string> List of quantities measurable by sensor
     */
    vector<string> Measurable_quantities() const;

    /**
     * @brief Create list of all available measurable quantities from numerical_quantity
     *
     * @return vector<string> List of numerical quantities measurable by sensor
     */
    vector<string> Measurable_numerical() const;

    /**
     * @brief Create list of all available measurable quantities from text_quantity
     *
     * @return vector<string> List of text quantities measurable by sensor
     */
    vector<string> Measurable_text() const;

    /**
     * @brief Create list of all available measurable quantities from other_quantity
     *
     * @return vector<string> List of other quantities measurable by sensor
     */
    vector<string> Measurable_others() const;

    /**
     * @brief   Perform measuring of selected numerical quantity
     *          Quantity must be one of Measurable numberical quantities
     *
     * @param value                 Name of numerical quantity
     * @return optional<double>     Value of quantity
     */
    optional<double> Measure_numerical(string quantity);

    /**
     * @brief   Perform measuring of selected quantity
     *          Quantity must be one of Measurable quantities
     *
     * @param value     Name of text quantity
     * @return optional<string>
     */
    optional<string> Measure_text(string quantity);

    /**
     * @brief   Created new record for quantity with method how to obtain it, used for numberical quantities
     *          Can create virtual file in component folder if filesystem is available in device
     *
     * @tparam class_T          Class from which is obtainer
     * @param quantity_name     Name of quantity which will be saved to measurable quantities
     * @param object            Pointer to object which provides method for obtaining value
     * @param method            Pointer to method of object above which provides value of quantity
     * @param create_file       If true, try to create virtual file in filesystem in component folder, can be used only when device has filesystem available
     * @return true             New record for quantity ais created
     * @return false            Record cannot be created, probably same quantity name already exists
     */
    template <typename class_T>
    bool Register_quantity(string quantity_name, class_T *object, double (class_T::*method)(void), bool create_file = false){
        if(Quantity_exists(quantity_name)){
            // Quantity with this name already exists
            return false;
        }
        numerical_quantity.insert(make_pair(quantity_name, new Invocation_wrapper<class_T, double, void>(object, method)));
        if (create_file){
            Create_virtual_file(quantity_name, object, method);
        }
        return true;
    }

    /**
     * @brief   Created new record for quantity with method how to obtain it, used for text quantities
     *          Can create virtual file in component folder if filesystem is available in device
     *
     * @tparam class_T          Class from which is obtainer
     * @param quantity_name     Name of quantity which will be saved to measurable quantities
     * @param object            Pointer to object which provides method for obtaining value
     * @param method            Pointer to method of object above which provides value of quantity
     * @param create_file       If true, try to create virtual file in filesystem in component folder, can be used only when device has filesystem available
     * @return true             New record for quantity ais created
     * @return false            Record cannot be created, probably same quantity name already exists
     */

    template <typename class_T>
    bool Register_quantity(string quantity_name, class_T *object, string (class_T::*method)(void), bool create_file = false){
        if(Quantity_exists(quantity_name)){
            // Quantity with this name already exists
            return false;
        }
        text_quantity.insert(make_pair(quantity_name, new Invocation_wrapper<class_T, string, void>(object, method)));
        if (create_file){
            Create_virtual_file(quantity_name, object, method);
        }
        return true;
    }

private:
    /**
     * @brief   Check if quantity with selected name already exists in any list of measurable quantity
     *          Try to find name in: numerical_quantity, text_quantity and other_quantity
     *
     * @param quantity_name     Name of quantity to check
     * @return true             Quantity already exists
     * @return false            Quantity does not exists at this sensor
     */
    bool Quantity_exists(string quantity_name);
};
