#include "directory.hpp"

Directory::Directory(string name){
    if (name.find("/") == string::npos){
        this->name = name;
    } else {
        name = "exception";
    }
    return -1;
}
