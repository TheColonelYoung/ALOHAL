#include "entry.hpp"

FS_entry::FS_entry(string name)
    :name(name){
}

string FS_entry::Path() const{
    string path;
    if(name == "/"){
        path = "/";
    } else {
        if (type == Type::Directory){
            path = parent->Path()+ name + "/" ;
        } else {
            path = parent->Path()+ name;
        }
    }
    return path;
}

bool FS_entry::Set_parent(FS_entry* entry){
    if(parent == nullptr){
        parent = entry;
        return true;
    } else {
        return false;
    }
}


void FS_entry::Delete(){
    this->~FS_entry();
}
