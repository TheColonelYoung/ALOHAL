#include "entry.hpp"

int FS_entry::Delete(){
    if (parent){
        parent.Delete_entry(this);
        return 0;
    }
    return -1;
}
