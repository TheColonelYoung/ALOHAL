/**
 * @file invocation_wrapper.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.3
 * @date 10.09.2019
 */

#pragma once

#include <functional>

/**
 * @brief   Base class for Invocation_wrapper, this class is used when inherited class
 *              Invocation_wrapper is saved to vector and have different class template
 *          Vector only needs to define return type and type of argument
 *
 * @tparam return_T     Template for type of returned value
 * @tparam args_T       Template for type of argument
 */
template <typename return_T, typename args_T>
class Invocation_wrapper_base {
public:
    Invocation_wrapper_base() = default;
    virtual ~Invocation_wrapper_base() = default;
    virtual return_T Invoke(args_T arg) const { return return_T(); };
    virtual bool operator == (const Invocation_wrapper_base<return_T, args_T> & compare) const { return false; };
};

/**
 * @brief   Same as Invocation_wrapper_base<return_T, args_T> but as argument type is used void
 *          Specialization of templated class above
 *
 * @tparam return_T Template for type of returned value
 */
template <typename return_T>
class Invocation_wrapper_base<return_T, void> {
public:
    Invocation_wrapper_base() = default;
    virtual ~Invocation_wrapper_base() = default;
    virtual return_T Invoke() const { return return_T(); };
    virtual bool operator == (const Invocation_wrapper_base<return_T, void> & compare) const { return false; };
};


/**
 * @brief   This wrapper hold pointer to object and method. Method must be assigned to class from which is object.
 *          Invoking of wrapper will pass arguments to stored method or pointer and then will run it.,
 *          After is encapsulated method of class done, will return value from it.
 *          Contains special variables for methods, which argument is void
 *
 * @tparam class_T   Type of class which is stored inside this wrapper
 * @tparam return_T  Type which will be returned from wrapped method or function
 * @tparam args_T    Type of input argument of method or function
 */
template <typename class_T, typename return_T, typename args_T>
class Invocation_wrapper : public Invocation_wrapper_base<return_T, args_T>
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

public:
    /**
     * @brief Construct a new Invocation_wrapper object which holds object and his class method
     *
     * @param object    Pointer to object which will be stored
     * @param method    Cointer to class method which can bee Invoked on object
     */
    Invocation_wrapper(class_T *object, return_T (class_T::*method) (args_T args)) :
        object_ptr(object), method_pointer(method)
    { }

    /**
     * @brief Invoke method on saved object or call saved function
     *
     * @param args          Arguments for encapsulated method
     * @return return_T     Value returned from encapsulated method
     */
    return_T Invoke(args_T args) const override final {
        if (object_ptr) {
            return (*object_ptr.*method_pointer)(args);
        } else {    // Return default value of type
            return return_T();
        }
    }

    /**
     * @brief   Perform comparission between two objects of class Invocation_wrapper_base,
     *              this operator is inherit and overloaded from base class
     *          Comparing object is cast to Invocation_wrapper
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */
    bool operator == (const Invocation_wrapper_base<return_T, args_T> & compare) const override final {
        auto compare_derivated = reinterpret_cast<const Invocation_wrapper<class_T, return_T, args_T> &>(compare);
        return (*this) == compare_derivated;
    }

    /**
     * @brief Perform comparission based on pointer to saved object and pointer to method
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */
    bool operator == (const Invocation_wrapper<class_T, return_T, args_T> & compare) const {
        return (this->object_ptr == compare.object_ptr) && (this->method_pointer == compare.method_pointer);
    }
};


/**
 * @brief   Specialized version of Invocation_wrapper which is used when method receive no arguments (void)
 *
 * @tparam class_T   Type of class which is stored inside this wrapper
 * @tparam return_T  Type which will be returned from wrapped method or function
 */
template <typename class_T, typename return_T>
class Invocation_wrapper<class_T, return_T, void> : public Invocation_wrapper_base<return_T, void>
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

public:
    /**
     * @brief   Construct a new Invocation_wrapper object which holds object and his class method
     *          This variant is used when method receive void as parametr
     *
     * @param object    Pointer to object which will be stored
     * @param method    Cointer to class method which can bee Invoked on object
     */
    Invocation_wrapper(class_T *object, return_T (class_T::*method) (void)) :
        object_ptr(object), method_pointer(method)
    { }

    /**
     * @brief   Invoke method on saved object
     *          This variant is used when method receive void as parametr
     *
     * @return return_T     Value returned from encapsulated method
     */
    return_T Invoke() const override final {
        if (object_ptr) {
            return (*object_ptr.*method_pointer)();
        } else {
            return return_T();
        }
    }

    /**
     * @brief   Perform comparission between two objects of class Invocation_wrapper_base,
     *              this operator is inherit and overloaded from base class
     *          Comparing object is cast to Invocation_wrapper
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */
    bool operator == (const Invocation_wrapper_base<return_T, void> & compare) const override final {
        auto compare_derivated = reinterpret_cast<const Invocation_wrapper<class_T, return_T, void> &>(compare);
        return (*this) == compare_derivated;
    }

    /**
     * @brief Perform comparission based on pointer to saved object and pointer to method
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */
    bool operator == (const Invocation_wrapper<class_T, return_T, void> & compare) const {
        return (this->object_ptr == compare.object_ptr) && (this->method_pointer == compare.method_pointer);
    }
};


/**
 * @brief   Specialized version of Invocation_wrapper which is used for function encapsulation (even lambdas)
 *
 * @tparam return_T  Type which will be returned from wrapped function
 * @tparam args_T    Type of input argument of function
 */
template <typename return_T, typename args_T>
class Invocation_wrapper<void, return_T, args_T> : public Invocation_wrapper_base<return_T, args_T>
{
private:

    /**
     * @brief   Encapsulated function
     */
    std::function<return_T(args_T)> *function;

public:

    /**
     * @brief Construct a new Invocation_wrapper object which holds function with no arguments
     *
     * @param function    Function wrapped in std:function
     */
    Invocation_wrapper(std::function<return_T(args_T)> *function) :
        function(function)
    { }

    /**
     * @brief   Destroy the Invocation_wrapper object and encapsulated function
     */
    ~Invocation_wrapper(){
        delete function;
    }

    /**
     * @brief Invoke saved function with given parameteres
     *
     * @param   args         Arguments for encapsulated function
     * @return  return_T     Value returned from encapsulated function
     */
    inline return_T Invoke(args_T args) const override final {
        return function(args);
    }

    /**
     * @brief   Perform comparission between two objects of class Invocation_wrapper_base,
     *              this operator is inherit and overloaded from base class
     *          Comparision is based only on pointer to this
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */

    bool operator == (const Invocation_wrapper_base<return_T, args_T> & compare) const override final {
        return (this) == &compare;
    }

    /**
     * @brief   Perform comparission between two objects of class Invocation_wrapper_base,
     *              this operator is inherit and overloaded from base class
     *          Comparision is based only on pointer to this
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */
    template <typename class_T>
    bool operator == (const Invocation_wrapper<class_T, return_T, args_T> & compare) const {
        return (this) == &compare;
    }
};


/**
 * @brief   Specialized version of Invocation_wrapper which is used for function (even lambdas) without arguments
 *
 * @tparam return_T  Type which will be returned from wrapped function
 * @tparam args_T    Type of input argument of function
 */
template <typename return_T>
class Invocation_wrapper<void, return_T, void> : public Invocation_wrapper_base<return_T, void>
{
private:

    /**
     * @brief   Encapsulated function
     */
    std::function<return_T()> *function;

public:

    /**
     * @brief Construct a new Invocation_wrapper object which holds function with no arguments
     *
     * @param function    Function wrapped in std:function
     */
    Invocation_wrapper(std::function<return_T()> *function) :
        function(function)
    { }

    /**
     * @brief   Destroy the Invocation_wrapper object and encapsulated function
     */
    ~Invocation_wrapper(){
        delete function;
    }

    /**
     * @brief Invoke saved function
     *
     * @return return_T     Value returned from encapsulated function
     */
    inline return_T Invoke() const override final {
        return (*function)();
    }

    /**
     * @brief   Perform comparission between two objects of class Invocation_wrapper_base,
     *              this operator is inherit and overloaded from base class
     *          Comparision is based only on pointer to this
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */

    bool operator == (const Invocation_wrapper_base<return_T, void> & compare) const override final {
        return (this) == &compare;
    }

    /**
     * @brief   Perform comparission between two objects of class Invocation_wrapper_base,
     *              this operator is inherit and overloaded from base class
     *          Comparision is based only on pointer to this
     *
     * @param compare   Right side of comparission
     * @return true     Object are same
     * @return false    Object are different
     */
    template <typename class_T, typename args_T>
    bool operator == (const Invocation_wrapper<class_T, return_T, args_T> & compare) const {
        return (this) == &compare;
    }
};
