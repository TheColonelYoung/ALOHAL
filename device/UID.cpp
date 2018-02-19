#include "UID.hpp"

namespace UID {
string UID_string;

void Init(){
    // Device = this;

    char str[10];
    sprintf(str, "%08lx", HAL_GetUIDw0());
    UID_string += str;

    sprintf(str, "%08lx", HAL_GetUIDw1());
    UID_string += str;

    sprintf(str, "%08lx", HAL_GetUIDw2());
    UID_string += str;
}

string String(int word){
    switch (word) {
        case 0:
            return UID_string.substr(0, 8);

        case 1:
            return UID_string.substr(8, 8);

        case 2:
            return UID_string.substr(16, 8);

        default:
            break;
    }
    return UID_string;
}
}
