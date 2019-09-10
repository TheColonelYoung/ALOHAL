/**
 * @file object_method_wrapper.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief
 * @version 0.1
 * @date 10.09.2019
 */

#pragma once

/**
 * @brief   This wrapper hold pointer to object and method. Method must be assigned to class from which is object.
 *          Also can be saved only function.
 *          Invoking of wrapper will pass arguments to stored method or pointer and then will run it.,
 *          After is encapsulated method of function done, will return value from it.
 *
 * @tparam class_T   Type of class which is stored inside this wrapper
 * @tparam return_T  Type which will be returned from wrapped method or function
 * @tparam args_T    Type of input argument of method or function
 */
template <typename class_T, typename return_T, typename args_T>
class Object_method_wrapper
{
private:

    /**
     * @brief Pointer to object on which will be method invocated
     */
    class_T *object_ptr = nullptr;

    /**
     * @brief pointer to method of class, which will be invocated on saved object
     */
    return_T (class_T::*method_pointer)(args_T args) = nullptr;

    /**
     * @brief Pointer to function which will be invocated
     */
    return_T (*function)(args_T args) = nullptr;

public:
    Object_method_wrapper() = default;

    /**
     * @brief Construct a new Object_method_wrapper object which holds object and his class method
     *
     * @param object    Pointer to object which will be stored
     * @param method    Cointer to class method which can bee Invoked on object
     */
    Object_method_wrapper(class_T *object, return_T(class_T::*method)(args_T args));

    /**
     * @brief   Construct a new Object_method_wrapper object which holds pointer to function
     *
     * @param function  Function to hold inside wrapper
     */
    Object_method_wrapper(return_T(*function)(args_T args));

    /**
     * @brief Invoke method on saved object or call saved function
     *
     * @param args          Arguments for encapsulated method
     * @return return_T     Value returned from encapsulated method
     */
    return_T Invoke(args_T &args) const {
        if (function) {
            return (*(function))(args);
        } else if (object_ptr) {
            return (*object_ptr.*method_pointer)(args);
        }
        return return_T();
    }
};
