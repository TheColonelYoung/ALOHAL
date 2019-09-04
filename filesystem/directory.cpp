#include "directory.hpp"

Directory::Directory(){
    type = Type::Directory ;
}

Directory::Directory(const string name){
    if (name.find("/") == string::npos){
        this->name = name;
    } else {
        this->name = "exception";
    }
}

FS_entry* Directory::Get_entry(const string entry_name) const{
    for(auto entry:content){
        if(entry->Name() == entry_name){
            return entry;
        }
    }
    return nullptr;
}

const bool Directory::Exists(const string filename){
    for(auto entry:content){
        if(entry->Name() == filename){
            return true;
        }
    }
    return false;
}
