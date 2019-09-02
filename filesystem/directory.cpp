#include "directory.hpp"

Directory::Directory(){
    type = Type::Directory ;
}

Directory::Directory(string name){
    if (name.find("/") == string::npos){
        this->name = name;
    } else {
        name = "exception";
    }
}
