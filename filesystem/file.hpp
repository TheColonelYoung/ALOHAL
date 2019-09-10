/**
 * @file file.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 05.09.2019
 */
#pragma once

#include "entry.hpp"
#include "misc/object_method_wrapper.hpp"

#include <string>

using namespace std;

/**
 * @brief   File is another type of filesystem entry, which can contain text
 *          Location of this test can be RAM, Flash or virtual
 *          Virtual means that data are obtained at time of reading of file
 *              - This can be used for reading data which are generated or need to be actual
 *
 * @tparam class_T Default template parametr for class, this template serve only when file is virtual
 *
 */
template <typename class_T = string>
class File : public FS_entry
{
public:
    enum class Location {
        RAM,    // Common variable
        Flash,  // Saved in MCu Flash memory, at the end after program
        Virtual // This is method which generates text message which can be printed
                //    - Can be used for actual data as temperature, which will be obtained during "reading the file"
    };
private:

    Object_method_wrapper<class_T, string, void(void)> *virtual_obtainer;

    Location location = Location::RAM;

    string content; // only for file located in RAM

public:
    File() = default;

    /**
     * @brief Construct a new File object which content is saved in RAM
     *
     * @param name      Name of file
     * @param content   Default content of file, can be empty
     */
    File(string name, string content = "") :
        location(Location::RAM),
        content(content){
        this->type = Type::File;
        this->name = name;
    }

    /**
     * @brief Construct a new File object which is virtual type
     *
     * @param name      Name of file
     * @param object    Pointer to object which will provide data for virtual file
     * @param method    Method of object which will provide data
     */
    File(string name, class_T *object, string (class_T::*method) (void)) :
        virtual_obtainer(new Object_method_wrapper<class_T, string, void>(object, method)){
        this->type = Type::File;
        this->name = name;
    }

    /**
     * @brief Write content of file, used when content is saved in RAM
     *
     * @param content   Content to write
     * @return int      0 if success, -1 when invalid Location type
     */
    int Write(string content){
        if (location != Location::RAM) {
            return -1;
        }

        this->content.assign(content);
        return 0;
    }

    /**
     * @brief Return content of file, if file is virtual, will call method for obtaining information
     *
     * @return string Content of file
     */
    string Read() const {
        if (location == Location::RAM) {
            return content;
        } else if (location == Location::Virtual) {
            return virtual_obtainer->Invoke();
        }

        return "";
    }

    /**
     * @brief   Return length of file content
     *          this can be only utilized when file is saved in RAM
     *
     * @return long Length of saved content, -1 is content is not saved in RAM
     */
    inline long Size() const {
        if (location == Location::RAM) {
            return content.length();
        } else {
            return -1;
        }
    };
};
