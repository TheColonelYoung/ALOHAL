#include "entry.hpp"

FS_entry::FS_entry(string name)
    :name(name){

}

const string FS_entry::Path(){
    string path;
    if(name == "/"){
        path = "";
    } else {
        path = parent->Path() + "/" + name;
    }
    return path;
}


int FS_entry::Delete(){

}
