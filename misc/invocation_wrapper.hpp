/**
 * @file invocation_wrapper.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief
 * @version 0.1
 * @date 10.09.2019
 */

#pragma once

#include <string>

using namespace std;

template <typename return_T, typename args_T>
class Invocation_wrapper_base{
public:
    Invocation_wrapper_base() =default;
    virtual ~Invocation_wrapper_base() =default;
    virtual return_T Invoke(args_T arg) const = 0;
    virtual bool operator==(const Invocation_wrapper_base<return_T, args_T> & compare) const = 0;
};

template <typename return_T>
class Invocation_wrapper_base<return_T, void>{
public:
    Invocation_wrapper_base() =default;
    virtual ~Invocation_wrapper_base() =default;
    virtual return_T Invoke() const = 0;
    virtual bool operator==(const Invocation_wrapper_base<return_T, void> & compare) const = 0;
};

/**
 * @brief   This wrapper hold pointer to object and method. Method must be assigned to class from which is object.
 *          Also can be saved only function.
 *          Invoking of wrapper will pass arguments to stored method or pointer and then will run it.,
 *          After is encapsulated method of function done, will return value from it.
 *          Contains special variables for methods, which argument is void
 *
 * @tparam class_T   Type of class which is stored inside this wrapper
 * @tparam return_T  Type which will be returned from wrapped method or function
 * @tparam args_T    Type of input argument of method or function
 */
template <typename class_T, typename return_T, typename args_T>
class Invocation_wrapper: public Invocation_wrapper_base<return_T, args_T>
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

    Invocation_wrapper() = default;

    /**
     * @brief Construct a new Invocation_wrapper object which holds object and his class method
     *
     * @param object    Pointer to object which will be stored
     * @param method    Cointer to class method which can bee Invoked on object
     */
    Invocation_wrapper(class_T *object, return_T(class_T::*method)(args_T args)) :
        object_ptr(object), method_pointer(method)
    { }

    /**
     * @brief   Construct a new Invocation_wrapper object which holds pointer to function
     *
     * @param function  Function to hold inside wrapper
     */
    Invocation_wrapper(return_T(*function)(args_T args)) :
        function(function)
    { }

    /**
     * @brief Invoke method on saved object or call saved function
     *
     * @param args          Arguments for encapsulated method
     * @return return_T     Value returned from encapsulated method
     */
    return_T Invoke(args_T args) const override final{
        if (function) {
            return (*(function))(args);
        } else if (object_ptr) {
            return (*object_ptr.*method_pointer)(args);
        }
        return return_T();
    }

    bool operator==(const Invocation_wrapper_base<return_T, args_T> & compare) const override final{
        auto compare_derivated = dynamic_cast<const Invocation_wrapper<class_T, return_T, args_T> &>(compare);
        return (*this)==compare_derivated;
    }

    bool operator==(const Invocation_wrapper<class_T, return_T, args_T> & compare) const{
        if(function){
            return this->function == compare.function;
        } else {
            return (this->object_ptr == compare.object_ptr) && (this->method_pointer==compare.method_pointer);
        }
    }
};

/**
 * @brief   Specialized version of class which is used when method receive void as argument
 *
 * @tparam class_T   Type of class which is stored inside this wrapper
 * @tparam return_T  Type which will be returned from wrapped method or function
 */
template <typename class_T, typename return_T>
class Invocation_wrapper<class_T, return_T, void>: public Invocation_wrapper_base<return_T, void>
{
private:

    /**
     * @brief Pointer to object on which will be method invocated
     */
    class_T *object_ptr = nullptr;

    /**
     * @brief pointer to method of class, which will be invocated on saved object
     */
    return_T (class_T::*method_pointer)(void) = nullptr;

    /**
     * @brief Pointer to function which will be invocated
     */
    return_T (*function)(void) = nullptr;

public:

    Invocation_wrapper() = default;

    /**
     * @brief   Construct a new Invocation_wrapper object which holds object and his class method
     *          This variant is used when method receive void as parametr
     *
     * @param object    Pointer to object which will be stored
     * @param method    Cointer to class method which can bee Invoked on object
     */
    Invocation_wrapper(class_T *object, return_T(class_T::*method)(void)) :
        object_ptr(object), method_pointer(method)
    { }

    /**
     * @brief   Construct a new Invocation_wrapper object which holds pointer to function
     *          This variant is used when method receive void as parametr
     *
     * @param function  Function to hold inside wrapper
     */
    Invocation_wrapper(return_T(*function)(void)) :
        function(function)
    { }

    /**
     * @brief   Invoke method on saved object or call saved function
     *          This variant is used when method receive void as parametr
     *
     * @return return_T     Value returned from encapsulated method
     */
    return_T Invoke() const override final{
        if (function) {
            return (*(function))();
        } else if (object_ptr) {
            return (*object_ptr.*method_pointer)();
        }
        return return_T();
    }

    bool operator==(const Invocation_wrapper_base<return_T, void> & compare) const override final{
        auto compare_derivated = dynamic_cast<const Invocation_wrapper<class_T, return_T, void> &>(compare);
        return (*this)==compare_derivated;
    }

    bool operator==(const Invocation_wrapper<class_T, return_T, void> & compare) const {
        if(function){
            return this->function == compare.function;
        } else {
            return (this->object_ptr == compare.object_ptr) && (this->method_pointer == compare.method_pointer);
        }
    }
};
