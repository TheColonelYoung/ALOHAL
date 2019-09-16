#include "directory.hpp"

Directory::Directory(){
    type = Type::Directory;
}

Directory::Directory(string name){
    if (name.find("/") == string::npos) {
        this->name = name;
    } else {
        if (name != "/") {
            this->name = "Exception";
        } else {
            this->name = "/";
        }
    }
    type = Type::Directory;
}

Directory::~Directory(){
    for(auto &entry:content){
        entry->Delete();
    }
}

FS_entry * Directory::Get_entry(const string entry_name) const {
    for (auto entry:content) {
        if (entry->Name() == entry_name) {
            return entry;
        }
    }
    return nullptr;
}

const bool Directory::Exists(const string filename){
    for (auto entry:content) {
        if (entry->Name() == filename) {
            return true;
        }
    }
    return false;
}

int Directory::Add_entry(FS_entry *entry){
    entry->Set_parent(this);
    content.emplace_back(entry);
    return content.size();
}

int Directory::Delete_entry(FS_entry *entry){
    for(auto &directory_entry:content){
        delete directory_entry;
    }
    return content.size();
}
